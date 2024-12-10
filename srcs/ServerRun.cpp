#include "../includes/IRC.hpp"

//Boucle principale qui fait tourner le serveur
void Server::run(){
    while (true) {
        int ep_w = epoll_wait(this->_epoll_fd, this->_events, this->_MAX_EVENTS, -1);
        if (ep_w == -1) {
            if (sig::stopServer) {
                break;
            } else {
                perror("epoll_wait");
                exit(EXIT_FAILURE);
            }
        }

        for (int i = 0; i < ep_w; ++i) {
            if (this->_events[i].data.fd == this->_server_fd) {
                acceptClients();
            } else {
                handleClient(this->_events[i].data.fd);
            }
        }
    }
}

//Accepte les nouvelles connexions client et les enregistre auprès d'epoll.
void Server::acceptClients() {
	struct sockaddr_in client_addr;
	memset(&client_addr, 0, sizeof(client_addr));
	socklen_t client_addr_len = sizeof(client_addr);
	int client_fd = accept(this->_server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
	if (client_fd == -1) {
		perror("accept");
		return;
	}

	Client* client = new Client(client_fd);
	this->_clients.insert(std::make_pair(client_fd, client));

	this->_ev.events = EPOLLIN | EPOLLET;
	this->_ev.data.fd = client_fd;
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, client_fd, &this->_ev) == -1) {
		perror("epoll_ctl: client_fd");
		return;
	}
//	//std::cout << "New client connected: fd " << client_fd << std::endl;
}

//Lis l'entree du client pour l'envoyer a la bonne fonction
void Server::handleClient(int client_fd) {
    char temp_buffer[1024];
    std::string accumulatedData;

    memset(temp_buffer, 0, sizeof(temp_buffer));

    while (accumulatedData.find("\r\n") == std::string::npos) {
        ssize_t bytesRead = recv(client_fd, temp_buffer, sizeof(temp_buffer) - 1, 0);

        if (bytesRead < 0) {
            perror("recv");
            closeClient(client_fd);
            return;
        }
        if (bytesRead == 0) {
            closeClient(client_fd);
            return;
        }
        accumulatedData.append(temp_buffer, bytesRead);
    }
    if (!accumulatedData.empty()) {
//        std::cout << RED << "handleClient Accumulated Data : " << accumulatedData << END << std::endl;
        processClientData(client_fd, accumulatedData, accumulatedData.size());
    }
}

void Server::processClientData(int client_fd, std::string& buffer, ssize_t count) {
    buffer.resize(count);

    std::vector<std::string> commands;
    std::string::size_type start = 0;
    std::string::size_type end;
    while ((end = buffer.find("\r\n", start)) != std::string::npos) {
        commands.push_back(buffer.substr(start, end - start));
        start = end + 2;
    }

    std::map<int, Client*>::iterator it = _clients.find(client_fd);
    if (it != _clients.end()) {
        for (size_t i = 0; i < commands.size(); ++i) {
            findCommand(commands[i], *it->second);
        }
    }
}


//Distribue la commande recue en entree dans la bonne fonction
void Server::findCommand(const std::string &command, Client &client) {
//    std::cout << MAGENTA << "findCommand Buffer : " << command << END << std::endl;

    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;
    if (cmd == "CAP") {
        return;
    } else if (cmd == "PASS") {
        pass(client, command, this->getPass());
    } else if (cmd == "NICK") {
        nick(client, command, _clients);
    } else if (cmd == "USER") {
        user(client, command);
    } else if (client.getAuthentication()) {
        if (cmd == "QUIT")
            quit(client, command, this);
        else if (cmd == "PART")
            part(client, this->_channels, command);
        else if (cmd == "OPER")
            oper(client, this->_channels, command);
        else if (cmd == "PRIVMSG")
            privMsg(client, this->_channels, this->_clients, command);
        else if (cmd == "JOIN")
            join(client, command, this->_channels);
        else if (cmd == "MODE")
            mode(client, this->_channels, this->_clients, command);
		else if (cmd == "INVITE")
			invite(client, this->_channels, command);
        else if (cmd == "KICK")
			kick(client, this->_channels, command);
        else if (cmd == "LIST")
			list(client, this->_channels);
        else if (cmd == "NAMES")
			names(client, this->_channels, command);
        else if (cmd == "TOPIC")
			topic(client, this->_channels, command);
        else if (cmd == "LUTIN")
			lutin(client);
        else if (cmd == "ROSES")
			roses(client);
        else if (cmd == "FIREBRO")
			fireBro(client);
        else if (cmd == "BOT")
			joinBotChannel(client, this->_channels);
        else if (cmd == "WHO")
            return ;
		else
            client.sendMessage(ERR_UNKNOWNCOMMAND(client.getNickname(), command));
    } else {
        client.sendMessage(ERR_NOTREGISTERED(client.getNickname()));
    }
}
