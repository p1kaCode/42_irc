/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 19:01:36 by ibenhaim          #+#    #+#             */
/*   Updated: 2024/02/29 14:29:16 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

void user(Client &client, const std::string &command) {
    if (client.getPass().empty()) {
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname()));
        return;
    }

    std::istringstream iss(command);
    std::string userCmd, username, zero, asterisk, realname;

    iss >> userCmd >> username >> zero >> asterisk;
    std::getline(iss, realname);

    if (!realname.empty() && realname[0] == ' ') {
        realname.erase(0, 1);
    }

    if (username.empty() || zero != "0" || asterisk != "*" || realname.empty()) {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "USER"));
        return;
    }

    if (!client.getUsername().empty()) {
        client.sendMessage(ERR_ALREADYREGISTERED(client.getNickname()));
        return;
    }

    client.setUsername(username);
    client.setRealname(realname);
    updateAuthenticationState(client, CMD_USER);
}
