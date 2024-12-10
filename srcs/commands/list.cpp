/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 10:37:18 by ibenhaim          #+#    #+#             */
/*   Updated: 2024/03/04 18:45:00 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

void	list(Client &client, std::map<std::string, Channel*> &channelList)
{	
	std::stringstream rplyList;

	client.sendMessage(RPL_LISTSTART(client.getNickname()));
	std::map<std::string, Channel*>::iterator it = channelList.begin();
	while (it != channelList.end())
	{
		if (it->second->getMode().find("s") == std::string::npos)
		{
			if (it->second->getTopic() == "")
				rplyList << "322 " << client.getNickname() << " #" << it->first << " " << it->second->getUsers().size() << ":No topic is set." << "\r\n" << std::endl;
			else
				rplyList << "322 " << client.getNickname() << " #" << it->first << " " << it->second->getUsers().size() << ":" << it->second->getTopic() << "\r\n" << std::endl;
			client.sendMessage(rplyList.str());
			rplyList.str("");
		}
		it++;
	}
	client.sendMessage(RPL_LISTEND(client.getNickname()));
}