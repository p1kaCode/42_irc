/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:33:53 by lmorel            #+#    #+#             */
/*   Updated: 2024/03/19 17:52:46 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

Channel::Channel(): _name("default"), _topic(""), _mode(""), _key(""), _userLimit(0)
{

}

Channel::Channel(const std::string name): _name(name), _topic(""), _mode(""), _key(""), _userLimit(0)
{
//	//std::cout << "Channel : " << this->_name << " created." << std::endl;
}

Channel::~Channel()
{
//	//std::cout << "Channel : "<< this->_name << " has been destroyed." << std::endl;
}

std::string			Channel::getName() const
{
	return (this->_name);
}

std::string			Channel::getTopic() const
{
	return (this->_topic);
}

std::map<std::string, Client*>		Channel::getUsers() const
{
	return (this->_users);
}

std::string	Channel::getKey() const
{
	return this->_key;
}

void	Channel::setKey(std::string& key)
{
	this->_key = key;
//	//std::cout << YELLOW << "Channel key set to -> " << key << RESET << std::endl;
}

unsigned int	Channel::getUserLimit() const
{
	return this->_userLimit;
}

void			Channel::setUserLimit(unsigned int limit)
{
	this->_userLimit = limit;
}

void	Channel::setTopic(std::string newTopic)
{
	this->_topic = newTopic;
}

void	Channel::addOperator(Client &user)
{
	if (this->_operators[user.getNickname()])
		return ;
	else
	{
		this->_operators[user.getNickname()] = &user;
		std::cout << "Added " << user.getNickname() << " to channels ops" << std::endl;
	}
}

void	Channel::rmOperator(Client &user)
{
	if (this->_operators[user.getNickname()])
	{
		this->_operators.erase(user.getNickname());
		std::cout << "Removed " << user.getNickname() << " of channel ops" << std::endl;
		int nbops = 0;
		std::map<std::string, Client*>::iterator check = this->_users.begin();
		while (check != this->_users.end())
		{
			if (isOperator(*check->second))
				nbops++;
			check++;
		}
		if (nbops == 0 && this->_users.size() > 0)
		{
			std::map<std::string, Client*>::iterator it = this->_users.begin();
			this->broadcastMessage(RPL_NEWCHANNELOP(this->getName(), "+o", it->first));
			this->addOperator(*it->second);
			std::cout << "\tops rights transfer" << std::endl;
		}
		else if (this->_users.size() > 0)
			std::cout << "\tstill ops in channel" << std::endl;
		else
			std::cout << "\tno more users in chan" << std::endl;
	}
	else
		return ;
}

bool	Channel::isOperator(Client &user)
{
	if (this->_operators[user.getNickname()])
		return true;
	else
		return false;
}

void	Channel::addMode(std::string const mode)
{
	if (this->_mode.empty() == true)
		this->_mode = mode;
	else
		_mode += mode;
}

void	Channel::rmMode(std::string const mode)
{
	if (this->_mode.find(mode) == std::string::npos)
		return ;
	size_t pos = this->_mode.find(mode);
	this->_mode.erase(pos, 1);
	return ;
}

std::string	Channel::getMode() const
{
	return (this->_mode);
}

std::map<std::string, Client*> Channel::getOperators() const
{
	return (this->_operators);
}

std::string	Channel::getUsersList(std::string askerNick)
{
	std::string	users = "";
	std::map<std::string,Client*>::iterator	it = this->_users.begin();
	while (it != this->_users.end())
	{
		if (this->_users[askerNick])
		{
			if (this->isOperator(*it->second))
				users.append("@" + (*it->second).getNickname() + " ");
			else
   				users.append((*it->second).getNickname() + " ");
			
		}
		else if ((*it->second).getMode().find("i") == std::string::npos)
		{
			if (this->isOperator(*it->second))
				users.append("@" + (*it->second).getNickname() + " ");
			else
   				users.append((*it->second).getNickname() + " ");
		}
		it++;
	}
	return users;
}

std::string	Channel::getSymbol() const
{
	std::string symbol = "";

	if (this->_mode.find('s') != std::string::npos) {
		symbol += "@";
	} else if (this->_mode.find('p') != std::string::npos) {
		symbol += "*";
	} else {
		symbol += "=";
	}
	return (symbol); 
}

void	Channel::join(Client &user)
{
	this->_users[user.getNickname()] = &user;
}

void	Channel::leave(Client &user)
{
	this->_users.erase(user.getNickname());
}

void	Channel::broadcastMessage(const std::string& msg)
{
	std::map<std::string,Client*>::iterator	it = this->_users.begin();
	while (it != this->_users.end())
	{
   		(*it->second).sendMessage(msg);
		it++;
	}
}

void	Channel::broadcastMessage(const std::string &msg, Client& exception)
{
	std::map<std::string,Client*>::iterator	it = this->_users.begin();
	while (it != this->_users.end())
	{
		if (!(*it->second == exception))
   			(*it->second).sendMessage(msg);
		it++;
	}
}

bool	Channel::hasUser(std::string nickname)
{
	// parcours les users et prints leurs nicknames lettres par lettres entourées de crochets
	for (std::map<std::string, Client*>::iterator it = this->_users.begin(); it != this->_users.end(); ++it) {
		if (it->first == nickname) {
			return true;
		}
	}
	return false;
}

std::vector<std::string>	Channel::getInvitedUsers() const
{
	return (this->_invitedUsers);
}

void	Channel::inviteUser(std::string nickname)
{
	this->_invitedUsers.push_back(nickname);
//	//std::cout << "Client " << nickname << " has been invited to channel " << this->_name << std::endl;
}

void	Channel::rmInvitedUser(std::string nickname)
{
	for (std::vector<std::string>::iterator it = this->_invitedUsers.begin(); it != this->_invitedUsers.end(); ++it) {
		if (*it == nickname) {
			this->_invitedUsers.erase(it);
			break;
		}
	}
}
