/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   protectedTopicMode.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 20:03:49 by lmorel            #+#    #+#             */
/*   Updated: 2024/03/19 17:43:27 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/IRC.hpp"

void	protectedTopicMode(Channel* channel, Client &client, std::string mode, std::string channelName)
{
	if (mode.find("+t") != std::string::npos)
	{
		if (channel->getMode().find("t") == std::string::npos)
		{
			channel->addMode("t");
			client.sendMessage(RPL_CHANNELMODEIS(client.getNickname(), channelName, mode));
			std::cout << "Client " << client.getNickname() << " set mode " << mode << " on channel " << channelName << std::endl;
		}
		else
		{
			std::cout << "Mode : " << mode << " was already on channel : " << channelName << std::endl;
			return ;
		}
	}
	else if (mode.find("-t") != std::string::npos)
	{
		if (channel->getMode().find("t") != std::string::npos)
		{
			channel->rmMode("t");
			client.sendMessage(RPL_CHANNELMODEIS(client.getNickname(), channelName, mode));
			std::cout << "Client " << client.getNickname() << " removed mode " << mode << " on channel " << channelName << std::endl;
		}
		else
			std::cout << "Mode : " << mode << " was not on channel : " << channelName << std::endl;
		return ;
	}
}