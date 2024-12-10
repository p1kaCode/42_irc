/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NumericalReplies.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 13:05:46 by ibenhaim          #+#    #+#             */
/*   Updated: 2024/03/19 17:34:19 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICALREPLIES_HPP
# define NUMERICALREPLIES_HPP

# include "IRC.hpp"

//SERVER NAME
# define SERVER_NAME std::string("FT_IRC")

//PART
# define RPL_PART(user_id, channel, reason) (user_id + " PART #" + channel + " " + reason + "\r\n")

# define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")
# define ERR_UNKNOWNCOMMAND(client, command) (":localhost 421 " + client + " " + command + " :Unknown command\r\n")

//PRIVMSG
# define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + username + "@localhost PRIVMSG " + target + " " + message + "\r\n")
# define ERR_NORECIPIENT(client) ("411 " + client + " :No recipient given PRIVMSG\r\n")

//NICK
# define RPL_CMD(nick, user, cmd, reply) (":" + nick + "!" + user + "@localhost" + " " + cmd + " " + reply + "\r\n")

//OPER
# define RPL_YOUREOPER(client) ("381 " + client + " :You are now an IRC operator\r\n")

//PASS
# define ERR_PASSWDMISMATCH(client) (":localhost 464 " + client + " :Password incorrect.\r\n")


// QUIT
# define RPL_QUIT(user_id, reason) (user_id + " QUIT :Quit: " + reason + "\r\n")
# define RPL_ERROR(user_id, reason) (user_id + " ERROR :" + reason + "\r\n")

//JOIN
# define RPL_JOIN(source, channel) (source + " JOIN :#" +  channel + "\r\n")
# define RPL_NAMREPLY(source, symbol, channel, users) (":localhost 353 " + source + " " + symbol + " #" + channel + " :" + users + "\r\n")
# define RPL_ENDOFNAMES(source, channel) (":localhost 366 " + source + " #" + channel + " :End of /NAMES list.\r\n")

//KICK
# define RPL_KICK(user_id, channel, kicked, reason) (user_id + " KICK #" + channel + " " + kicked + " reason : " + reason + "\r\n")

//TOPIC
# define RPL_NOTOPIC(client, channel) (":localhost 331 " + client + " #" + channel + " :No topic is set\r\n")
# define RPL_TOPIC(client, channel, topic) (":localhost 332 " + client + " #" + channel + " " + topic + "\r\n")

//LIST
# define RPL_LISTSTART(client) (":localhost 321 " + client + " Channel :Users Name\r\n")
# define RPL_LISTEND(client) (":localhost 323 " + client + " :End of /LIST\r\n")

//INVITE
# define RPL_INVITING(client, nickname, channel) (":localhost 341 " + client + " " + nickname + " #" + channel + "\r\n")
# define ERR_USERONCHANNEL(client, nickname, channel) (":localhost 443 " + client + " " + nickname + " #" + channel + " :is already on channel\r\n")

//MODE
# define RPL_CHANNELMODEIS(client, channel, mode) (":localhost 324 " + client + " #" + channel + " " + mode + "\r\n")
# define RPL_NEWCHANNELOP(channel, mode, nickname) (":localhost MODE #" + channel + " " + mode + " " + nickname + "\r\n")
# define RPL_CHANNELMODEIS(client, channel, mode) (":localhost 324 " + client + " #" + channel + " " + mode + "\r\n")

//ERR
# define ERR_INVITEONLYCHAN(client, channel) (":localhost 473 " + client + " #" + channel + " :Cannot join channel (+i)\r\n")
# define ERR_NOPRIVILEGES(client) ("481 " + client + " :Permission Denied- You're not an IRC operator\r\n")
# define ERR_NOSUCHCHANNEL(client, channel) (":localhost 403 " + client + " #" + channel + " :No such channel\r\n")
# define ERR_USERNOTINCHANNEL(client, nickname, channel) ("441 " + client + " " + nickname + " #" + channel + " :They aren't on that channel\r\n")
# define ERR_NOTONCHANNEL(client, channel) (":localhost 442 " + client + " #" + channel + " :The user is not on this channel.\r\n")
# define ERR_INVALIDKEY(client, channel) (":localhost 525 " + client + " #" + channel + " :Key is not well-formed, key can only contains letters\r\n")
# define ERR_CHANOPRIVSNEEDED(client, channel) (":localhost 482 " + client + " #" + channel + " :You're not channel operator\r\n")
# define ERR_BADCHANNELKEY(client, channel) (":localhost 475 " + client + " #" + channel + " :Cannot join channel (+k)\r\n")
# define ERR_CHANNELISFULL(client, channel) (":localhost 471 " + client + " #" + channel + " :Cannot join channel (+l)\r\n")
# define ERR_BANNEDFROMCHAN(client, channel) (":localhost 474 " + client + " #" + channel + " :Cannot join channel (+b)\r\n")
# define ERR_NOSUCHNICK(client, target) ("401 " + client + " " + target + " :No such nick/channel\r\n")
# define ERR_NICKNAMEINUSE(nick) (":" + SERVER_NAME + " 433 " + nick + " :Nickname is already in use\r\n")
# define ERR_NONICKNAMEGIVEN(nick) (":" + SERVER_NAME + " 431 " + nick + " :No nickname given\r\n")
# define ERR_ERRONEUSNICKNAME(nick) (":" + SERVER_NAME + " 432 " + nick + " :Erroneus nickname\r\n")
# define ERR_ALREADYREGISTERED(nick) (":" + SERVER_NAME + " 462 " + nick + " :You may not reregister\r\n")
# define ERR_NEEDMOREPARAMS(nick, command) (":" + SERVER_NAME + " 461 " + nick + " " + command + " :Not enough parameters\r\n")
# define RPL_TRYAGAIN(nick, command) (":" + SERVER_NAME + " 263 " + nick + " " + command + " :Invalid parameters, try again\r\n")
# define ERR_NOTREGISTERED(nick) (":" + SERVER_NAME + " 451 " + nick + " :You have not registered\r\n")
# define RPL_NICK(previousNick, newNick, user) (":" + previousNick + "!" + user + "@localhost" + " NICK " + newNick + "\r\n")

#endif