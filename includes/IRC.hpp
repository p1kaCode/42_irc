/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:58:39 by lmorel            #+#    #+#             */
/*   Updated: 2024/02/28 21:32:55 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <cerrno>
#include <sstream>
#include <map>
#include <vector>
#include <sstream>
#include <fcntl.h>
#include <sys/epoll.h>
#include <csignal>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


# define END		"\033[0m"
# define RESET		"\033[0m"
# define BLACK		"\033[30m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define MAGENTA	"\033[35m"
# define CYAN		"\033[36m"
# define WHITE		"\033[37m"

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Commands.hpp"
#include "NumericalReplies.hpp"

namespace sig
{
    extern bool stopServer;
    void signalHandler(int signum);
}

namespace sig
{
    extern bool stopServer;
    void signalHandler(int signum);
}

#endif
