/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 12:44:33 by ibenhaim          #+#    #+#             */
/*   Updated: 2024/03/20 16:29:04 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

void kick(Client &client, std::map<std::string, Channel*> &channelList, std::string buffer) {
	std::istringstream iss(buffer);
	std::string command, channelName, nickname, reason;
	std::string tmp;
	std::vector<std::string> nicknames;

	// Extraction des données du buffer
	iss >> command >> channelName >> nickname;
	nicknames = getChannels(nickname);
	std::getline(iss, reason);
	reason = reason.substr(reason.find(":") + 1);
	// std::cout << "Command: " << command << std::endl;
	// std::cout << "Channel: " << channelName << std::endl;
	// std::cout << "Nickname: " << nickname << std::endl;
	// std::cout << "Reason: " << reason << std::endl;
	Channel *channel = NULL;
	if (channelName[0] == '#')
		channelName.erase(0, 1);
	
	// afficher le nom de tout les channels
	for (std::map<std::string, Channel*>::iterator it = channelList.begin(); it != channelList.end(); ++it) {
		std::istringstream iss(it->first);
		iss >> tmp;
		if (tmp == channelName) {
			channel = it->second;
			break;
		}
	}
	for (std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); it++)
	{
		nickname = *it;
		// Envoyer un message d'erreur au client si le canal n'existe pas
		if (channel == NULL) {
			client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
			// std::cout << "Channel non trouvé" << std::endl;
			return;
		}
	//	//std::cout << "Channel trouvé" << std::endl;
		
		if (!channel->hasUser(nickname)) {
			client.sendMessage(ERR_USERNOTINCHANNEL(client.getNickname(), nickname, channel->getName()));
			// std::cout << "User non trouvé" << std::endl;
			return;
		}
	//	//std::cout << "User trouvé" << std::endl;


		if (channel->isOperator(client) == true) {
			// kick le nickname du channel
			std::cout << "Kicking user : "<< nickname << " from " + channel->getName() << "." << std::endl;
			if (channel->isOperator(*channel->getUsers()[nickname]) == true)
				channel->rmOperator(*channel->getUsers()[nickname]);
			channel->broadcastMessage(RPL_KICK(user_id(client.getNickname(), client.getUsername()), channelName, nickname, reason));
			channel->leave(*channel->getUsers()[nickname]);
			
		} else {
	//		//std::cout << "No privileges" << std::endl;
			client.sendMessage(ERR_NOPRIVILEGES(client.getNickname()));
		}
	}
}