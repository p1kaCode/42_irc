/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:09:10 by lmorel            #+#    #+#             */
/*   Updated: 2024/03/20 16:56:22 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

void    names(Client &client, std::map<std::string, Channel*> &channelList, std::string buffer)
{
	std::istringstream iss(buffer);
	std::string command, channelName;
	std::vector<std::string> channels;

	// Extraction des données du buffer
	iss >> command >> channelName;
	channels = getChannels(channelName);
	
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		channelName = *it;
		// std::cout << MAGENTA << "DEBUG NAMES -> ChannelName: " << channelName << RESET << std::endl;
		// If the channel name is invalid or the channel does not exist, one RPL_ENDOFNAMES numeric containing the given channel name should be returned.
		if (channelList.find(channelName) == channelList.end())
		{
			client.sendMessage(RPL_ENDOFNAMES(client.getNickname(), channelName));
			return ;
		}
		Channel*	chan = channelList[channelName];

		// If the given channel has the secret channel mode set and the user is not joined to that channel, one RPL_ENDOFNAMES numeric is returned.
		if (chan->getMode().find("s") == std::string::npos && chan->getUsers()[client.getNickname()])
		{
			client.sendMessage(RPL_ENDOFNAMES(client.getNickname(), chan->getName()));
			return ;
		}
		client.sendMessage(RPL_NAMREPLY(client.getNickname(), chan->getSymbol(), chan->getName(), chan->getUsersList(client.getNickname())));
		client.sendMessage(RPL_ENDOFNAMES(client.getNickname(), chan->getName()));
	}
}