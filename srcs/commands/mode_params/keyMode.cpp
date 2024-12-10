/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 20:26:37 by lmorel            #+#    #+#             */
/*   Updated: 2024/03/20 16:15:50 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/IRC.hpp"

void keyMode(Channel* channel, Client &client, std::string mode, std::string key)
{
	if (mode.find("+k") != std::string::npos)
	{
		if (key == "")
		{
			client.sendMessage(ERR_INVALIDKEY(client.getNickname(), channel->getName()));
			return ;
		}
		for (std::string::iterator i = key.begin(); i != key.end(); i++)
		{
			if (!isalpha(*i))
			{
				client.sendMessage(ERR_INVALIDKEY(client.getNickname(), channel->getName()));
				return ;
			}
		}
		if (channel->getMode().find("k") == std::string::npos)
		{
			channel->addMode("k");
			client.sendMessage(RPL_CHANNELMODEIS(client.getNickname(), channel->getName(), mode));
		}
		channel->setKey(key);
		std::cout << "Client " << client.getNickname() << " set mode " << mode << " on channel " << channel->getName() << " , key is now : " << channel->getKey() << "."<< std::endl;
		return ;
	}
	else if (mode.find("-k") != std::string::npos)
	{
		if (channel->getMode().find("k") != std::string::npos)
		{
			channel->rmMode("k");
			client.sendMessage(RPL_CHANNELMODEIS(client.getNickname(), channel->getName(), mode));
			std::cout << "Client " << client.getNickname() << " removed mode " << mode << " on channel " << channel->getName() << "." << std::endl;
		}
//		//std::cout << "Mode : " << mode << " was not on channel : " << channel->getName() << std::endl;
		return ;
	}
}