/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 11:27:08 by ibenhaim          #+#    #+#             */
/*   Updated: 2024/03/20 16:24:48 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

void	invite(Client &client, std::map<std::string, Channel*> &channelList, std::string buffer)
{
	std::istringstream iss(buffer);
	std::string command, nickname, channelName;
	std::string tmp;

	// Extraction des données du buffer
	iss >> command >> nickname >> channelName;
	
	Channel *channel = NULL;
	if (channelName[0] == '#')
		channelName.erase(0, 1);
	// afficher le nom de tout les channels
    if (command.empty() || nickname.empty() || channelName.empty()) {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE"));
        return;
    }

    for (std::map<std::string, Channel*>::iterator it = channelList.begin(); it != channelList.end(); ++it) {
		std::istringstream iss(it->first);
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
	
	if (!channel->hasUser(client.getNickname())) {
//		//std::cout << "Client " << client.getNickname() << " is not on channel " << channelName << std::endl;
		client.sendMessage(ERR_NOTONCHANNEL(client.getNickname(), channelName));
		return;
	}

	if (channel->getMode().find("i") != std::string::npos && channel->isOperator(client) == false) {
//		//std::cout << "Client " << client.getNickname() << " is not operator on channel " << channelName << std::endl;
		client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
		return;
	}

	if (channel->hasUser(nickname)) {
//		//std::cout << "Client " << nickname << " is already on channel " << channelName << std::endl;
		client.sendMessage(ERR_USERONCHANNEL(client.getNickname(), nickname, channelName));
		return;
	}

	// Envoyer un message d'invitation au client
	client.sendMessage(RPL_INVITING(client.getNickname(), nickname, channelName));
	
	// Envoyer un message d'invitation à tout les clients sur le canal
	channel->inviteUser(nickname);
	
	std::cout << "Client " << client.getNickname() << " invited " << nickname << " to channel " << channelName << std::endl;
}