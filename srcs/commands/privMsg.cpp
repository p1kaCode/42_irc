/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 11:23:12 by ibenhaim          #+#    #+#             */
/*   Updated: 2024/03/15 15:17:50 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

void privMsg(Client &client, std::map<std::string, Channel*> &channelList, std::map<int, Client*> &clientList, std::string str) {
	int pos = str.find("PRIVMSG");

	std::string target;
	std::string msg;

	size_t spacePos = str.find(" ", pos);
	size_t colonPos = str.find(":", pos);

	if (spacePos != std::string::npos && colonPos != std::string::npos) {
		target = str.substr(spacePos, colonPos - spacePos);
	} else {
		client.sendMessage(ERR_NORECIPIENT(client.getNickname()));
		return;
	}
	
	//trim spaces in target
	for (size_t i = 0; i < target.size(); i++) {
		if (target[i] == ' ') {
			target.erase(i, 1);
			i--;
		}
	}

	colonPos = str.find(":", pos);
	// size_t endPos = str.find("\r\n");
	
	if (colonPos != std::string::npos) {
		msg = str.substr(colonPos + 1);
	} else {
		client.sendMessage(ERR_NORECIPIENT(client.getNickname()));
		return;
	}
	
	//std::cout << "target: " << target << std::endl;
	
	if (target.find("#") != std::string::npos)
	{
		target.erase(0, 1);
		//std::cout << "sending message to channel " << target << std::endl;
		Channel *channel = NULL;
		for (std::map<std::string, Channel*>::iterator it = channelList.begin(); it != channelList.end(); ++it) {
			std::istringstream iss(it->first);
			std::string tmp;
			iss >> tmp;
			if (tmp == target) {
				channel = it->second;
				// BONUS
				if (channel->getName() == BOTNAME)
				{
					//std::cout << "Client wants to talk to JARVIS" << std::endl;
					talkToBot(client, channel, msg);
					return ;
				}
				// END BONUS
				//std::cout << "channel found" << std::endl;
				break;
			}
		}
		if (channel == NULL || channel->getUsers().size() == 0) {
			//std::cout << "Channel " << target << " does not exist" << std::endl;
			client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), target));
			return;
		}
		if (!channel->getUsers()[client.getNickname()]) {
			//std::cout << "Channel " << target << " does not exist" << std::endl;
			client.sendMessage(ERR_NOTONCHANNEL(client.getNickname(), channel->getName()));
			return;
		}
		target.insert(0, "#");
		std::map<std::string, Client*> users = channel->getUsers();
		for (std::map<std::string, Client*>::iterator it = users.begin(); it != users.end(); ++it) {
			if (it->first != client.getNickname()) {
				//std::cout << "sending message to " << it->first << std::endl;
				it->second->sendMessage(RPL_PRIVMSG(client.getNickname(), client.getUsername(), target, msg));
			}
		}
	}
	else
	{
		//std::cout << "sending message to user " << target << std::endl;

		for (std::map<int, Client*>::iterator it = clientList.begin(); it != clientList.end(); ++it) {
			if (it->second->getNickname() == target) {
				it->second->sendMessage(RPL_PRIVMSG(client.getNickname(), client.getUsername(), target, msg));
				return;
			}
		}
		client.sendMessage(ERR_NOSUCHNICK(client.getNickname(), target));
	}
}