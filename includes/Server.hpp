#ifndef SERVER_HPP
# define SERVER_HPP
#include "IRC.hpp"

class Client;
class Channel;

class Server {

public:
    Server(int port, const std::string& password);
    ~Server();

    void                    run();
    void                    closeClient(int client_fd);
    std::map<std::string, Channel*> getChannels() const {
        return _channels;
    }
    std::string             getPass() const	{return this->_password;};

private:
    int                     _server_fd;
    int                     _epoll_fd;
    int                     _port;
    std::string             _password;
    std::map<int, Client*>  _clients;
    std::map<std::string, Channel*>      _channels;
    static const int        _MAX_EVENTS = 10;
    struct epoll_event      _ev;
    struct epoll_event      _events[_MAX_EVENTS];

    //////////////////SETUP SERVER //////////////////////////////
    void                    setupServerSocket();
    void                    setupEpoll();

    //////////////////////SETUP RUN//////////////////////////////
    void                    acceptClients();
    void                    handleClient(int client_fd);
    void                    findCommand(const std::string &command, Client &client);
    void                    processClientData(int client_fd, std::string& buffer, ssize_t count);

    ///////////////////////////DESTRUCTOR//////////////////////////
    void                    mapCLientDestruction();
    void                    channelDestruction();
    void                    destructionEpoll();

};

#endif