/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   roses.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 18:58:24 by ibenhaim          #+#    #+#             */
/*   Updated: 2024/02/25 14:40:14 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

void roses(Client &client) { // send flowers to the client
	client.sendMessage(RPL_PRIVMSG(client.getNickname(), client.getUsername(), client.getNickname(), "🌹Thanks for the Roses🌹")); 
}

void lutin(Client &client) { // send le lutin est aux arrivants to the client
	client.sendMessage(RPL_PRIVMSG(client.getNickname(), client.getUsername(), client.getNickname(), "🎅Le lutin est aux arrivants🎅")); 
}

void fireBro(Client &client) { // send fireBro to the client avec des flammes et des coeurs
	client.sendMessage(RPL_PRIVMSG(client.getNickname(), client.getUsername(), client.getNickname(), "🔥FireBro💖")); 
}