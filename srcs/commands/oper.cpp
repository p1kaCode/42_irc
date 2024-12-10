/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 11:48:20 by ibenhaim          #+#    #+#             */
/*   Updated: 2024/03/08 13:37:05 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

void oper(Client &client, std::map<std::string, Channel *> &channelList, std::string buffer) {
    std::istringstream iss(buffer);
    std::string command, username, password;

    // Extraction des données du buffer
    iss >> command >> username >> password;

    if (command.empty() || username.empty() || password.empty()) {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "OPER"));
        return;
    }

    // Vérification des informations de l'utilisateur
    if (username == "admin" && password == "admin") {
        client.addMode("o");
        client.sendMessage(RPL_YOUREOPER(client.getNickname()));
        for (std::map<std::string, Channel*>::iterator it = channelList.begin(); it != channelList.end(); ++it) {
            Channel* channel = it->second;
            if (channel->isOperator(client) == false) {
                channel->addOperator(client);
	        	channel->broadcastMessage(RPL_NEWCHANNELOP(channel->getName(), "+o", client.getNickname()));
            }
        }
    } else {
        client.sendMessage(ERR_PASSWDMISMATCH(client.getNickname()));
    }
}