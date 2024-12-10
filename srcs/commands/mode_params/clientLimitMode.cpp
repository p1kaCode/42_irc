/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientLimitMode.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 20:17:46 by lmorel            #+#    #+#             */
/*   Updated: 2024/03/20 16:15:02 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/IRC.hpp"

void	clientLimitMode(Channel* channel, Client &client, std::string mode, int limit)
{
	if (mode.find("+l") != std::string::npos)
	{
		if (channel->getMode().find("l") == std::string::npos)
			channel->addMode("l");
		channel->setUserLimit(limit);
		client.sendMessage(RPL_CHANNELMODEIS(client.getNickname(), channel->getName(), mode));
		std::cout << "Client " << client.getNickname() << " set mode " << mode << " on channel " << channel->getName() << " , limit is now : " << channel->getUserLimit() << std::endl;
		return ;
	}
	else if (mode.find("-l") != std::string::npos)
	{
		if (channel->getMode().find("l") != std::string::npos)
		{
			channel->rmMode("l");
			client.sendMessage(RPL_CHANNELMODEIS(client.getNickname(), channel->getName(), mode));
		}
		channel->setUserLimit(0);
		std::cout << "Client " << client.getNickname() << " removed mode " << mode << " on channel " << channel->getName() << " , limit is now : " << channel->getUserLimit() << std::endl;
		return ;
	}
}
