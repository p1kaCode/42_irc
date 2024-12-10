/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 13:07:04 by ibenhaim          #+#    #+#             */
/*   Updated: 2024/03/20 16:00:45 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

void    mode(Client &client, std::map<std::string, Channel*> &channelList, std::map<int, Client*> &clientList, const std::string &buffer) {

	std::istringstream iss(buffer);
	std::string command, channelName, mode, arg;

	// Extraction des données du buffer
	iss >> command >> channelName >> mode >> arg;
	if (channelName[0] == '#')
		channelName.erase(0, 1);
	if (mode.empty() == false && mode[0] == ':')
		mode.erase(0, 1);
//	//std::cout << "channelName:" << channelName << "| mode:" << mode << "| arg :" << arg << "|" << std::endl;
	
	Channel *channel = NULL;
	// afficher le nom de tout les channels
	for (std::map<std::string, Channel*>::iterator it = channelList.begin(); it != channelList.end(); ++it) {
		std::istringstream iss(it->first);
		std::string tmp;
		iss >> tmp;
		if (tmp == channelName) {
			channel = it->second;
			break;
		}
	}

	if (channel == NULL || channel->getUsers().size() == 0) {
//		//std::cout << "Channel " << channelName << " does not exist" << std::endl;
		client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
		return;
	}

	if (mode.empty() || !(mode[0] == '-' || mode[0] == '+'))
	{
		client.sendMessage(RPL_CHANNELMODEIS(client.getNickname(), channel->getName(), channel->getMode()));
		return ;
	}

	if (channel->isOperator(client) == false) {
		client.sendMessage(ERR_NOPRIVILEGES(client.getNickname()));
		return;
	}

	if (!channel->hasUser(client.getNickname())) {
//		//std::cout << "Client " << client.getNickname() << " is not on channel " << channelName << std::endl;
		client.sendMessage(ERR_NOTONCHANNEL(client.getNickname(), channelName));
		return;
	}

    if (mode.length() != 2 || !(mode[0] == '-' || mode[0] == '+')) {
        client.sendMessage(RPL_TRYAGAIN(client.getNickname(), "MODE"));
        return;
    }

	// CHANNEL INVITE ONLY MODE +i
	if (mode.find("i") != std::string::npos)
		inviteMode(*channel, client, mode, channelName);

	//PROTECTED TOPIC MODE +t
	if (mode.find("t") != std::string::npos)
		protectedTopicMode(channel, client, mode, channelName);

	// CHANNEL KEY MODE +k
	if (mode.find("k") != std::string::npos) 
		keyMode(channel, client, mode, arg);
		
	// OPERATOR +o
	if (mode.find("o") != std::string::npos)
		operatorsMode(*channel, client, mode, channelName, clientList, arg);

	// CHANNEL CLIENT LIMIT MODE +l
	if (mode.find("l") != std::string::npos)
		clientLimitMode(channel, client, mode, atoi(arg.c_str()));
	
}