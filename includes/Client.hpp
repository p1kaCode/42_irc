/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:04:51 by lmorel            #+#    #+#             */
/*   Updated: 2024/03/04 20:03:04 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include "IRC.hpp"

class Client {
public:

    Client(int socket);		//Constructor
    ~Client();			//Destructor

    /////////////////////// FUNCTIONS ///////////////////////
    void	sendMessage(const std::string& message);
    void    addMode(std::string mode);

    //////////////////////// GETTERS /////////////////////////
    int			getSocket()		const 	{return this->_clientSocket;};
    std::string	getNickname()	const	{return this->_nickname;};
    std::string	getUsername()	const	{return this->_username;};
    std::string	getRealname()	const	{return this->_realname;};
    std::string	getPass()		const	{return this->_pass;};
    bool        getAuthentication() const {return this->_isAuthenticated;};
	int			getAuthState()	const	{return this->_authState;};
    std::string getMode() const {return this->_mode;};
    std::string getPreviousNick() const {return this->_previousNick;};

    //////////////////////// SETTERS ////////////////////////
    void	setClientSocket(int socket)	{this->_clientSocket = socket;};
    void	setNickname(std::string nickname)	{this->_nickname = nickname;};
    void	setUsername(std::string username)	{this->_username = username;};
    void	setRealname(std::string realname)	{this->_realname = realname;};
	void	setAuthentication(bool auth)		{this->_isAuthenticated = auth;};
	void	setPass(std::string pass)			{this->_pass = pass;};
	void	setAuthState(int state)				{this->_authState = state;};
    void    setPreviousNick(std::string nickname) {this->_previousNick = nickname;};

private:

    int				_clientSocket;
    std::string		_username;
    std::string		_realname;
    std::string		_nickname;
    std::string		_pass;
    std::string     _mode;
    std::string     _previousNick;
    bool            _isAuthenticated;
	int				_authState;

    // FUNCTIONS
    void retrieveUserinfos(std::string str);
    void tryPasswd(std::string str, int *tries, std::string pass);
};

bool operator==(const Client& rhs, const Client& lhs);

#endif