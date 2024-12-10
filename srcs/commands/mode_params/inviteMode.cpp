/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inviteMode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 12:04:44 by ibenhaim          #+#    #+#             */
/*   Updated: 2024/03/20 16:13:43 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/IRC.hpp"

void inviteMode(Channel &channel, Client &client, std::string mode, std::string channelName)
{		
	if (channel.isOperator(client) == false) {
//		//std::cout << "Client " << client.getNickname() << " is not operator on channel " << channelName << std::endl;
		client.sendMessage(ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName));
		return;
	}
	
	if (mode.find("+i") != std::string::npos) {
		if (channel.getMode().find("i") != std::string::npos) {
//			//std::cout << "Channel " << channelName << " is already in Invite-Only Mode." << std::endl;
			return;
		}
		else
		{
			channel.addMode("i");
			client.sendMessage(RPL_CHANNELMODEIS(client.getNickname(), channelName, mode));
			std::cout << "Client " << client.getNickname() << " set mode " << mode << " on channel " << channelName << std::endl;
		}
	}

	else if (mode.find("-i") != std::string::npos) {
		if (channel.getMode().find("i") == std::string::npos) {
//			//std::cout << "Channel " << channelName << " is not in Invite-Only Mode." << std::endl;
			return;
		}
		else
		{
			client.sendMessage(RPL_CHANNELMODEIS(client.getNickname(), channelName, mode));
			std::cout << "Client " << client.getNickname() << " removed mode " << mode << " on channel " << channelName << std::endl;
			channel.rmMode("i");
		}
	}	
}
