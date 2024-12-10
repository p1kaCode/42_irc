/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:34:17 by lmorel            #+#    #+#             */
/*   Updated: 2024/03/20 16:34:18 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

void quit(Client& client, const std::string& buffer, Server* server) {
   std::string quitMsg = "Client quit";

   std::size_t pos = buffer.find("QUIT");
   if (pos != std::string::npos) {
       pos = buffer.find(" :", pos);
       if (pos != std::string::npos) {
           quitMsg = buffer.substr(pos + 2);
       }
   }
    std::string::iterator i = quitMsg.begin();
    int k = 0;
    while (i != quitMsg.end())
    {
        if (!isalnum(*i) && *i != ' ' && *i != '\'' && *i != '\"')
            break;
        i++;
        k++;
    }
    std::string reason;
    if (i != quitMsg.end())
        reason = quitMsg.substr(0, k);

    std::string nick = client.getNickname();
    std::string user = client.getUsername();

    client.sendMessage(RPL_QUIT(user_id(nick, user), reason));

    std::map<std::string, Channel*> channels = server->getChannels();
    for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
        Channel* channel = it->second;
        channel->leave(client);
        if (channel->isOperator(client)) {
            channel->rmOperator(client);
        } 
        channel->broadcastMessage(RPL_QUIT(user_id(nick, user), reason), client);
    }

    server->closeClient(client.getSocket());
}
