/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 21:02:25 by lmorel            #+#    #+#             */
/*   Updated: 2024/03/20 16:51:00 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

void	part_all(Client &client, std::map<std::string, Channel*> &channelList)
{
	std::map<std::string, Channel*>::iterator it = channelList.begin();
	std::string buf;

	while (it != channelList.end())
	{
		if ((*it->second).hasUser(client.getNickname()))
		{
			buf.append("PART #");
			buf.append(it->first);
			std::cout << buf << std::endl;
			part(client, channelList, buf);
			buf.clear();
		}
		it++;
	}
}

void	join(Client &client, std::string buffer, std::map<std::string, Channel*> &channelList)
{
//    std::cout << MAGENTA << "Buffer JOIN : " << buffer << END << std::endl;
//	//std::cout << YELLOW "JOIN PARSING\nReceived : " + buffer;
	while (buffer.find("<") !=  std::string::npos)
		buffer.erase(buffer.find("<"));
	while (buffer.find(">") !=  std::string::npos)
			buffer.erase(buffer.find(">"));
	std::istringstream iss(buffer);
	std::vector<std::string> channelNames, keys;
	std::string cmd, s_chan, s_key;
	iss >> cmd >> s_chan >> s_key;
	if (s_chan.empty())
	{
		client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "JOIN #channel (key)"));
		return ;
	}
	if (s_chan == "0")
	{
		part_all(client, channelList);
		return ;
	}
	channelNames = getChannels(s_chan);
	if (!s_key.empty())
		keys = getChannels(s_key);
	
	std::vector<std::string>::iterator channelName, key;
	bool isChannelCreator = false;
	channelName = channelNames.begin();
	key = keys.begin();
	int diff = keys.size() - channelNames.size();
	if (channelName == channelNames.end() || diff > 0)
	{
		client.sendMessage(RPL_TRYAGAIN(client.getNickname(), "JOIN #channel key"));
		return ;
	}
	while (channelName != channelNames.end())
	{
		if ((*channelName)[0] == '#')
			*channelName = (*channelName).substr(1);
//		//std::cout << MAGENTA << "DEBUG JOIN -> ChannelName: " << *channelName << RESET << std::endl;
		// si le chan n'existe pas on le cree
		if (channelList.find(*channelName) == channelList.end())
		{
			Channel*	newChan = new Channel(*channelName);
			channelList[*channelName] = newChan;
		}

		// trouve le chan dans la liste et on appelle la fonction pour le rejoindre
		Channel*	chan = channelList[*channelName];
		if (chan->getUsers().size() == 0)
			isChannelCreator = true;

		if (chan->hasUser(client.getNickname()) == true)
		{
			client.sendMessage(ERR_USERONCHANNEL(client.getNickname(), client.getNickname(), chan->getName()));
			return ;
		}

		// USER LIMIT MODE
		if (chan->getMode().find("l") && chan->getUserLimit() > 0)
		{
			if (chan->getUserLimit() < chan->getUsers().size() + 1)
			{
				client.sendMessage(ERR_CHANNELISFULL(client.getNickname(), chan->getName()));
				return ;
			}
//			//std::cout << MAGENTA << "DEBUG JOIN -> " << chan->getUserLimit() - (chan->getUsers().size()) << " slots left, joining channel ..." << RESET << std::endl;
		}

		// KEY MODE
		if (chan->getMode().find("k") != std::string::npos && chan->getKey() != "")
		{
			if (key == keys.end() || *key != chan->getKey())
			{
				client.sendMessage(ERR_BADCHANNELKEY(client.getNickname(), chan->getName()));
				return ;
			}
			std::cout << YELLOW << "JOIN : Good key, joining channel ..." << RESET << std::endl;
		}
		//  INVITE ONLY MODE
		if (chan->getMode().find("i") != std::string::npos)
		{
			// si le client n'est pas opérateur sur le channel
			if (client.getMode().find("o") == std::string::npos)
			{
				// parcourir le vecteur de clients invités pour savoir si il a été invité
				std::vector<std::string> invitedUsers = chan->getInvitedUsers();
				std::vector<std::string>::iterator it;
				for (it = invitedUsers.begin(); it != invitedUsers.end(); ++it)
				{
//					//std::cout << "invited user : " << *it << std::endl;
					if (*it == client.getNickname())
						break;
				}
				// si le client n'a pas été invité
				if (it == chan->getInvitedUsers().end())
				{
					// le serveur envoie un message d'erreur au client
//					//std::cout << "Client " << client.getNickname() << " is not invited on channel " << chan->getName() << std::endl;
					client.sendMessage(ERR_INVITEONLYCHAN(client.getNickname(), chan->getName()));
					return;
				}
				else
				{
					// le client est invité, on le retire de la liste des invités
					chan->rmInvitedUser(client.getNickname());
				}
			}
		}

		chan->join(client);
		// le serveur confirme l'entree dans le channel au client
		client.sendMessage(RPL_JOIN(user_id(client.getNickname(), client.getUsername()), chan->getName()));

		if (chan->getTopic() != "")
			client.sendMessage(RPL_TOPIC(client.getNickname(), chan->getName(), chan->getTopic()));
		else
			client.sendMessage(RPL_NOTOPIC(client.getNickname(), chan->getName()));

		client.sendMessage(RPL_NAMREPLY(client.getNickname(), chan->getSymbol(), chan->getName(), chan->getUsersList(client.getNickname())));
		client.sendMessage(RPL_ENDOFNAMES(client.getNickname(), chan->getName()));

		// le serveur annonce l'arrive du client dans le channel
		chan->broadcastMessage(RPL_JOIN(user_id(client.getNickname(), client.getUsername()), chan->getName()), client);
		// message de debug console du serveur
		std::cout << YELLOW << "CLient " << client.getNickname() << " joined channel " << chan->getName() << RESET << std::endl;

		if (isChannelCreator == true || client.getMode().find("o") != std::string::npos)
		{
			chan->addOperator(client);
			chan->broadcastMessage(RPL_NEWCHANNELOP(chan->getName(), "+o", client.getNickname()));
			if (chan->getMode().find("t") == std::string::npos)
				chan->addMode("t");
		}
		channelName++;
		key++;
	}
}
