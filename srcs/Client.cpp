/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:31:21 by ibenhaim          #+#    #+#             */
/*   Updated: 2024/03/20 16:55:26 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

Client::Client(int socket) : _clientSocket(socket), _isAuthenticated(false), _authState(0) {

}

Client::~Client() {
    //close(socket);
}

void Client::sendMessage(const std::string& message) {
    size_t totalSent = 0;
    ssize_t sent;
    while (totalSent < message.length()) {
        sent = send(this->_clientSocket, message.c_str() + totalSent, message.length() - totalSent, MSG_NOSIGNAL);
        if (sent < 0) {
            perror("Erreur lors de l'envoi send()");
            break;
        }
        totalSent += sent;
    }
    if (totalSent < message.length()) {
        std::cerr << "Attention : l'intégralité du message n'a pas été envoyée." << std::endl;
    }
}


void Client::tryPasswd(std::string str, int *tries, std::string pass)
{
//    //std::cout << MAGENTA << "PASS : " << END << str << std::endl;
    if (str.compare(pass) != 0)
    {
        // std::cout << RED << "Wrong password." << END << std::endl;
        *tries += 1;
        return ;
    }
    this->_pass = str;
    return ;
}


void Client::retrieveUserinfos(std::string str)
{
    std::istringstream iss(str);
    std::string token;

    // Get the username
    std::getline(iss, token, ' ');
    this->_username = token;

    // Skip the next two tokens
    std::getline(iss, token, ' ');
    std::getline(iss, token, ' ');

    // Get the real name
    if (std::getline(iss, token, ':'))
    {
        std::getline(iss, token);
        this->_realname = token;
    }
}

void Client::addMode(std::string mode) {
    if (this->_mode.empty())
        this->_mode = "+" + mode;
    else
        this->_mode += mode;
}

bool operator==(const Client& rhs, const Client& lhs)
{
	return (rhs.getSocket() == lhs.getSocket());
};