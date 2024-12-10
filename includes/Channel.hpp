/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:51:03 by lmorel            #+#    #+#             */
/*   Updated: 2024/03/19 16:44:39 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "IRC.hpp"

class Client;

class Channel
{
   private:
       Channel();
		std::string						_name;
		std::string						_topic;
		std::string						_mode;
		std::string						_key;
		std::map<std::string, Client*>	_users;
		std::map<std::string, Client*>	_operators;
		std::vector<std::string>		_invitedUsers;
		std::vector<std::string>		_bannedUsers;
		unsigned int					_userLimit;


   public:
		Channel(std::string name);
		~Channel();

		std::string							getName() const;
		std::string							getTopic() const;
		std::map<std::string, Client*>		getUsers() const;
		std::string							getUsersList(std::string askerNick);
		std::string							getSymbol() const;
		std::string							getMode() const;
		std::vector<std::string>			getInvitedUsers() const;
		std::string							getKey() const;
		unsigned int						getUserLimit() const;
		std::map<std::string, Client*>		getOperators() const;

		void				setUserLimit(unsigned int limit);
		void				setKey(std::string& key);
		void				setTopic(std::string newTopic);
		void				addMode(std::string const mode);
		void				rmMode(std::string const mode);
		void				addOperator(Client &user);
		void				rmOperator(Client &user);
		bool				isOperator(Client &user);
		bool				hasUser(std::string nickname);
		void				inviteUser(std::string nickname);
		void				rmInvitedUser(std::string nickname);

		void	join(Client &user);
		void	leave(Client &user);
		void	broadcastMessage(const std::string &msg);
		void	broadcastMessage(const std::string &msg, Client& exception);

};

#endif