/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 17:13:11 by ibenhaim          #+#    #+#             */
/*   Updated: 2024/03/05 11:47:14 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

static void         checkAndSetNickname(const std::string &nickname, Client &client, const std::map<int, Client*>& clients);
static bool         isNicknameValid(const std::string& nickname);
static bool         isNicknameUsed(const std::string& nickname, const std::map<int, Client*>& clients);

void nick(Client &client, std::string buffer, const std::map<int, Client*>& clients) {
    std::string command, nickname;
    std::istringstream iss(buffer);
    iss >> command >> nickname;
    if (nickname.empty()) {
//        std::cerr << "Erreur lors du set de nickname from buffer: nickname vide" << std::endl;
        client.sendMessage(ERR_NONICKNAMEGIVEN());
        return;
    }
    checkAndSetNickname(nickname, client, clients);
}


static void checkAndSetNickname(const std::string &nickname, Client &client, const std::map<int, Client*>& clients) {
    if (client.getPass().empty()) {
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname()));
        return;
    }
    if (nickname.empty()) {
        client.sendMessage(ERR_NONICKNAMEGIVEN(std::string("Client")));
        return;
    }
    if (!isNicknameValid(nickname)) {
        client.sendMessage(ERR_ERRONEUSNICKNAME(nickname));
        return;
    }
    if (isNicknameUsed(nickname, clients)) {
        if (client.getPreviousNick().empty()) {
            client.setPreviousNick(nickname);
        }
        client.sendMessage(ERR_NICKNAMEINUSE(nickname));
        return;
    }
    client.setNickname(nickname);
    std::string finalNick;
    if (client.getPreviousNick().empty()) {
        finalNick = client.getNickname();
    } else {
        finalNick = client.getPreviousNick();
    }
    client.sendMessage(RPL_CMD(finalNick, client.getUsername(), "NICK", client.getNickname()));
    updateAuthenticationState(client, CMD_NICK);
}

static bool isNicknameValid(const std::string& nickname) {
    std::string	forbiddenAll = " .,*?!@";
    std::string	forbiddenFirst = "$:#&~%+";
    if (nickname.find_first_of(forbiddenAll) != std::string::npos)
        return false;
    if (forbiddenFirst.find(nickname[0]) != std::string::npos)
        return false;
    if (nickname.empty() || nickname.length() > 15)
        return false;
    for (std::string::const_iterator it = nickname.begin(); it != nickname.end(); ++it) {
        if (!std::isalnum(*it) && *it != '_') {
            return false;
        }
    }
    return true;
}

static bool isNicknameUsed(const std::string& nickname, const std::map<int, Client*>& clients) {
    for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->second->getNickname() == nickname) {
            return true;
        }
    }
    return false;
}
