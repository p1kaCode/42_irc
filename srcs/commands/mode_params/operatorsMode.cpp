/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operatorsMode.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 14:45:58 by ibenhaim          #+#    #+#             */
/*   Updated: 2024/03/20 16:16:31 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/IRC.hpp"

void operatorsMode(Channel &channel, Client &client, std::string mode, std::string channelName, std::map<int, Client*> &clientList, std::string nickname)
{
	Client *target = NULL;

	if (channel.isOperator(client) == false) {
//		//std::cout << "Client " << client.getNickname() << " is not operator on channel " << channelName << std::endl;
		client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
		return;
	}
	
	for (std::map<int, Client*>::iterator it = clientList.begin(); it != clientList.end(); ++it) {
		if (it->second->getNickname() == nickname) {
			target = it->second;
			break;
		}
	}
	if (target == NULL) {
//		//std::cout << "Client " << nickname << " does not exist" << std::endl;
		client.sendMessage(ERR_NOSUCHNICK(client.getNickname(), nickname));
		return;
	}
	
	if (mode.find("+o") != std::string::npos) {
		if (channel.isOperator(*target) == true) {
//			//std::cout << "Client " << client.getNickname() << " is already operator on channel " << channelName << std::endl;
			return;
		}
		else
		{
			channel.addOperator(*target);
			channel.broadcastMessage(RPL_NEWCHANNELOP(channel.getName(), "+o", target->getNickname()));
			std::cout << "Client " << client.getNickname() << " set mode " << mode << " on client " << target->getNickname() << std::endl;
		}
	}
	else if (mode.find("-o") != std::string::npos) {
		if (channel.isOperator(*target) == false) {
//			//std::cout << "Client " << client.getNickname() << " is not operator on channel " << channelName << std::endl;
			return;
		}
		else
		{
			if (channel.getOperators().size() > 1)
			{
				channel.rmOperator(*target);
				channel.broadcastMessage(RPL_NEWCHANNELOP(channel.getName(), "-o", target->getNickname()));
				std::cout << "Client " << client.getNickname() << " removed mode " << mode << " on client " << target->getNickname() << std::endl;
			}
			else
				client.sendMessage(RPL_ERROR(user_id(target->getNickname(), target->getUsername()), "You are the last operator on this channel."));
		}
	}
}	