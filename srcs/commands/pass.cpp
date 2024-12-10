/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:16:00 by ibenhaim          #+#    #+#             */
/*   Updated: 2024/03/20 16:57:09 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

void pass(Client &client, const std::string &command, const std::string &serverPass) {
    std::istringstream iss(command);
    std::string cmd;
    std::string clientPass;

    iss >> cmd;

    std::getline(iss >> std::ws, clientPass);

    if (client.getAuthentication()) {
        client.sendMessage(ERR_ALREADYREGISTERED(client.getNickname()));
        return;
    }

    if (clientPass.empty()) {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "PASS"));
        return;
    }

    if (clientPass == serverPass) {
        client.setPass(clientPass);
        updateAuthenticationState(client, CMD_PASS);
        std::cout << GREEN << "Password validated." << END << std::endl;
    } else {
        client.sendMessage(ERR_PASSWDMISMATCH(client.getNickname()));
        std::cout << RED << "Wrong password." << END << std::endl;
    }
}

