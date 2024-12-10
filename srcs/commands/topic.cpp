/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 18:58:42 by lmorel            #+#    #+#             */
/*   Updated: 2024/03/20 16:43:15 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

void    topic(Client &client, std::map<std::string, Channel*> &channelList, std::string buffer)
{
	std::istringstream iss(buffer);
	std::string command, channelName, topic;
	
	// Extraction des données du buffer
	topic = "";
	iss >> command >> channelName;
	if (channelName[0] == '#')
		channelName.erase(0, 1);
	
	if (channelList.find(channelName) == channelList.end())
	{
		client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
		return ;
	}
	Channel*	chan = channelList[channelName];
	// CHECK IF USER IS IN CHAN
	if (chan->hasUser(client.getNickname()) == false)
	{
		client.sendMessage(ERR_NOTONCHANNEL(client.getNickname(), chan->getName()));
		return ;
	}
	bool validTopic = true;
	if (buffer.find(":") == std::string::npos)
		validTopic = false;
	else
	{
		topic = buffer.substr(buffer.find(":")+2, buffer.length());
		std::string::iterator i = topic.begin();
		while (i != topic.end())
		{
			if (isalnum(*i))
				break ;
			i++;
		}
		if (i == topic.end())
		{
			chan->setTopic("");
			validTopic = false;
		}
	}
	//std::cout << MAGENTA << "DEBUG TOPIC -> ChannelName: " << channelName << "| topic : " << topic << RESET << std::endl;
	// if protected topic mode client should have permissions or ERR_CHANOPRIVSNEEDED is returned
	if (chan->getMode().find("t") != std::string::npos && validTopic)
	{
		if (client.getMode().find("o") != std::string::npos)
			chan->setTopic(topic);
		else if (chan->isOperator(client) == true)
			chan->setTopic(topic);
		else
		{
			client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), chan->getName()));
			return ;
		}
	}
	else if (validTopic)	// if all conditions are skipped, change the topic
		chan->setTopic(topic);
	
	// IF SUCCESS RPL_TOPIC
	if (chan->getTopic() != "")
		client.sendMessage(RPL_TOPIC(client.getNickname(), chan->getName(), chan->getTopic()));
	else
		client.sendMessage(RPL_NOTOPIC(client.getNickname(), chan->getName()));
	if (validTopic)
	{
		// BROADCAST LE TOPIC A TT lES UTILISATEURS DU CHAN
		if (chan->getTopic() != "")
			chan->broadcastMessage(RPL_TOPIC(client.getNickname(), chan->getName(), chan->getTopic()), client);
		else
			chan->broadcastMessage(RPL_NOTOPIC(client.getNickname(), chan->getName()), client);
		std::cout << "Client " + client.getNickname() + " set " + chan->getName() + "'s topic to " + topic + "." << std::endl;
	}
}