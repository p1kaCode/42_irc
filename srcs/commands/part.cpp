/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 20:18:53 by lmorel            #+#    #+#             */
/*   Updated: 2024/03/20 16:31:54 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

void    part(Client &client, std::map<std::string, Channel*> &channelList, std::string buffer)
{
	std::istringstream iss(buffer);
	std::string command, channelName, quitMsg;
	std::vector<std::string> channels;

	// Extraction des données du buffer
	iss >> command >> channelName;
	channels = getChannels(channelName);
	// reason
	std::getline(iss, quitMsg);
	quitMsg = quitMsg.substr(quitMsg.find(":") + 1);
	// std::cout << MAGENTA << "DEBUG PART -> command: " << command << "| channelName: " << channelName << "| quitMsg: " << quitMsg << RESET << std::endl;
	// parcourir et afficher channels
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		//devbug reason
		// std::cout << MAGENTA << "DEBUG PART -> quitMsg: " << quitMsg << RESET << std::endl;
		
		// on enleve les #
		channelName = *it;
		if (channelName[0] == '#')
			channelName = channelName.substr(1);
		if (quitMsg.empty())
			quitMsg = "Leaving";

		// check if channel exists
		if (channelList.find(channelName) == channelList.end())
		{
			client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
			return ;
		}
		Channel*	chan = channelList[channelName];
		// check if user is in channel
		if (chan->hasUser(client.getNickname()) == false)
		{
			client.sendMessage(ERR_NOTONCHANNEL(client.getNickname(), chan->getName()));
			return ;
		}
		chan->leave(client);
		chan->rmOperator(client);
		std::cout << YELLOW << "Client " + client.getNickname() + " left " << channelName << ", reason : " << quitMsg << RESET << std::endl;
		client.sendMessage(RPL_PART(user_id(client.getNickname(), client.getUsername()), chan->getName(), quitMsg));
		chan->broadcastMessage(RPL_PART(user_id(client.getNickname(), client.getUsername()), chan->getName(), quitMsg), client);
	}

	
	
}