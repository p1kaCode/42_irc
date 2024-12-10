#include "../includes/IRC.hpp"

Server::Server(int port, const std::string& password) : _port(port), _password(password) {
    setupServerSocket();
    setupEpoll();
}

Server::~Server() {
    mapCLientDestruction();
    destructionEpoll();
    channelDestruction();
}

//Configure le socket serveur pour écouter sur le port spécifié et se prépare à accepter les connexions entrantes.
void Server::setupServerSocket() {
    this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(this->_port);

    int opt = 1;

    if (setsockopt(this->_server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed"); 
        close(this->_server_fd);
        exit(EXIT_FAILURE);
    }

    if (bind(this->_server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(this->_server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(this->_server_fd, SOMAXCONN) == -1) {
        perror("listen");
        close(this->_server_fd);
        exit(EXIT_FAILURE);
    }
}

//Initialise epoll pour la gestion asynchrone des sockets, y compris le socket serveur.
void Server::setupEpoll() {
    this->_epoll_fd = epoll_create1(0);
    if (this->_epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    memset(&this->_ev, 0, sizeof(this->_ev));

    this->_ev.events = EPOLLIN;
    this->_ev.data.fd = this->_server_fd;
    if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_server_fd, &this->_ev) == -1) {
        perror("epoll_ctl: _server_fd");
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, sig::signalHandler);
}

void Server::closeClient(int client_fd) {
    std::cout << RED << "The client linked to the fd " << client_fd << " has left..." << END << std::endl;
    if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, client_fd, NULL) == -1) {
        perror("epoll_ctl: EPOLL_CTL_DEL client_fd");
    }
    close(client_fd);

    std::map<int, Client*>::iterator it = this->_clients.find(client_fd);
    if (it != this->_clients.end()) {
        delete it->second;
        this->_clients.erase(it);
    }
}

void Server::mapCLientDestruction() {
    std::map<int, Client*>::iterator it;
    for (it = this->_clients.begin(); it != this->_clients.end(); ++it) {
        close(it->first);
        delete it->second;
    }
    this->_clients.clear();
}

void Server::destructionEpoll() {
    if (this->_server_fd != -1) {
        close(_server_fd);
    }
    if (this->_epoll_fd != -1) {
        close(_epoll_fd);
    }
}

void Server::channelDestruction() {
    std::map<std::string, Channel*>::iterator itChannel;
    for (itChannel = this->_channels.begin(); itChannel != this->_channels.end(); ++itChannel) {
        delete itChannel->second;
    }
    this->_channels.clear();
}