/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 17:14:00 by ibenhaim          #+#    #+#             */
/*   Updated: 2024/03/19 18:10:53 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP
# define BOTNAME "JARVIS"

#include "IRC.hpp"

///////////////////////////COMMANDS/////////////////////////////////

void	joinBotChannel(Client& client, std::map<std::string, Channel*> &channelList);
void    talkToBot(Client& client, Channel* channel, std::string msg);
void    notice(Client &client, std::string &buffer);
void    topic(Client &client, std::map<std::string, Channel*> &channelList, std::string buffer);
void	invite(Client &client, std::map<std::string, Channel*> &channelList, std::string buffer);
void    oper(Client &client, std::map<std::string, Channel*> &channelList, std::string buffer);
void    privMsg(Client &client, std::map<std::string, Channel*> &channelList, std::map<int, Client*> &clientList, std::string str);
void 	pass(Client &client, const std::string &command, const std::string &serverPass);
void 	nick(Client &client, std::string buffer, const std::map<int, Client*>& clients);
void 	user(Client &client, const std::string &command);
void 	quit(Client& client, const std::string& command, Server* server);
void    kick(Client &client, std::map<std::string, Channel*> &channelList, std::string buffer);
void	list(Client &client, std::map<std::string, Channel*> &channelList);
void	join(Client &client, std::string buffer, std::map<std::string, Channel*> &channelList);
void	part(Client &client, std::map<std::string, Channel*> &channelList, std::string buffer);
void    names(Client &client, std::map<std::string, Channel*> &channelList, std::string buffer);
void    mode(Client &client, std::map<std::string, Channel*> &channelList, std::map<int, Client*> &clientList, const std::string &buffer);
void    inviteMode(Channel &channel, Client &client, std::string mode, std::string channelName); // +i et -i
void    protectedTopicMode(Channel* channel, Client &client, std::string mode, std::string channelName); // +t and -t
void    keyMode(Channel* channel, Client &client, std::string mode, std::string key);	// +k and -k
void    operatorsMode(Channel &channel, Client &client, std::string mode, std::string channelName, std::map<int, Client*> &clientList, std::string nickname); // +o and -o
void    clientLimitMode(Channel* channel, Client &client, std::string mode, int limit); // +l and -l
void    roses(Client &client);
void    lutin(Client &client);
void    fireBro(Client &client);
		

///////////////////////////UTILS/////////////////////////////////

enum CommandType {
	CMD_PASS,
	CMD_NICK,
	CMD_USER
};

void						updateAuthenticationState(Client &client, CommandType commandType);
std::vector<std::string>	getChannels(const std::string &s);

#endif