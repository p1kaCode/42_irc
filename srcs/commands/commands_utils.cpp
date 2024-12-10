/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 17:40:22 by lmorel            #+#    #+#             */
/*   Updated: 2024/03/20 16:57:03 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

// BONUS

# define FORMAT "curl -s https://api.openai.com/v1/chat/completions \
		-H \"Content-Type: application/json\" \
		-H \"Authorization: Bearer " + apikey + "\" \
		-d '" + "{\"model\":\"gpt-3.5-turbo-16k\",\"messages\":[{\"role\": \"system\",\"content\": \"You are my assistant that , but you can answer only 500 caracters maximum\"},{\"role\":\"user\",\"content\":\"" + msg + "\"}]}" + "' | jq '.choices[].message.content'"

# define APIKEY "HIDDEN"

    // API CHECK
int    apicheck(std::string apikey)
{
    std::string check = "curl -s https://api.openai.com/v1/models -H \"Authorization: Bearer " + apikey + "\" | jq '.error'";
	FILE*	pipe = popen(check.c_str(), "r");
	if (!pipe) {
		std::cerr << RED << "Error popen()" << END << std::endl;
		return 1;
	}

	char	buff[256];
	std::string res;

	while (fgets(buff, 256, pipe) != NULL) {
		if (ferror(pipe)) {
			std::cerr << RED << "Error fgets()" << END << std::endl;
			return 1;
		}
		res += buff;
	}
	pclose(pipe);
	if (res == "null\n")
    {
//		std::cerr << GREEN << "API READY" << END << std::endl;
        return 0;
    }
    else
    {
        std::cerr << RED << "API NOT READY" << END << std::endl;
        return 1;
    }
}

void    talkToBot(Client& client, Channel* channel, std::string msg)
{
    // send msg to API
    while (msg.find('"') != std::string::npos)
        msg.replace(msg.find('"'), 1, " ");
    while (msg.find('\'') != std::string::npos)
        msg.replace(msg.find('\''), 1, " ");
    
    std::string apikey = APIKEY;
    std::string command = FORMAT;
//    //std::cout << RED << command.c_str() << std::endl;
   	FILE*	pipe = popen(command.c_str(), "r");
	if (!pipe) {
		std::cerr << RED << "Error popen()" << END << std::endl;
		return ;
	}

	char	buff[256];
	std::string res;

	while (fgets(buff, 256, pipe) != NULL) {
		if (ferror(pipe)) {
			std::cerr << RED << "Error fgets()" << END << std::endl;
			return ;
		}
		res += buff;
	}
	pclose(pipe);
    // retreive answer and send it to client via channel
    std::string botName = BOTNAME;
    std::string target = channel->getName();
    target.insert(0, "#");
    if (res.size() == 0)
    {
        client.sendMessage(RPL_PRIVMSG(botName, botName, target, BOTNAME + " is unavailable now."));
        return ;
    }
//    //std::cout << YELLOW << "-- ANSWER RECIEVED -- " << std::endl << "->" + res << RESET << std::endl;
    res.erase(res.find_first_of('"'), 1);
    res.erase(res.find_last_of('"'), 1);
    client.sendMessage(RPL_PRIVMSG(botName, botName, target, res));
}

void	joinBotChannel(Client& client, std::map<std::string, Channel*> &channelList)
{
    if (apicheck(APIKEY) == 1){
        client.sendMessage(ERR_NOSUCHCHANNEL(client.getNickname(), BOTNAME));
        return ;
    }
	std::string channelName = BOTNAME;
	if (channelList.find(channelName) == channelList.end())
	{
		Channel*	newChan = new Channel(channelName);
		channelList[channelName] = newChan;
        std::string topic = BOTNAME;
        topic.append(" IS HERE TO HELP");
        newChan->setTopic(topic);
        newChan->addMode("t");
	}
	Channel*	chan = channelList[channelName];

	chan->join(client);
	client.sendMessage(RPL_JOIN(user_id(client.getNickname(), client.getUsername()), chan->getName()));
	if (chan->getTopic() != "")
		client.sendMessage(RPL_TOPIC(client.getNickname(), chan->getName(), chan->getTopic()));
	else
		client.sendMessage(RPL_NOTOPIC(client.getNickname(), chan->getName()));
	client.sendMessage(RPL_NAMREPLY(client.getNickname(), chan->getSymbol(), chan->getName(), chan->getUsersList(client.getNickname())));
	client.sendMessage(RPL_ENDOFNAMES(client.getNickname(), chan->getName()));
}
// END BONUS


void updateAuthenticationState(Client &client, CommandType commandType) {
    switch (commandType) {
        case CMD_PASS:
            if (client.getAuthState() == 0) {
                client.setAuthState(1);
                // std::cout << GREEN << "Password validated." << END << std::endl;
            }
            break;
        case CMD_NICK:
        case CMD_USER:
            if (client.getAuthState() == 1 && !client.getAuthentication()) {
                client.setAuthState(2);
//                //std::cout << GREEN << "Nickname/User validated after password." << END << std::endl;
            }
            break;
    }

    if (client.getAuthState() == 2 && (!client.getNickname().empty() && !client.getUsername().empty())) {
        client.setAuthentication(true);
        client.setAuthState(3);
        std::cout << GREEN << "Client : " + client.getNickname() + " is fully authenticated." << END << std::endl;
    }
}

std::vector<std::string>	getChannels(const std::string &s)
{
    std::vector<std::string>	channels;
    std::string::size_type		pos = 0;
    std::string::size_type		prev = 0;

    // on va passer de #salon1,#salon2,#salon3 à salon1, salon2, salon3
    while ((pos = s.find(",", prev)) != std::string::npos)
    {
        channels.push_back(s.substr(prev, pos - prev));
        prev = pos + 1;
    }
    channels.push_back(s.substr(prev));
    return (channels);    
}
