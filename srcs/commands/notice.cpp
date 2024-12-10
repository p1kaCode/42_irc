/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 12:55:37 by ibenhaim          #+#    #+#             */
/*   Updated: 2024/03/15 15:18:23 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRC.hpp"

void notice(Client &client, std::string &buffer) {
	std::string::size_type pos = buffer.find("NOTICE");
	if (pos != std::string::npos)
	{
		std::string msg = buffer.substr(buffer.find(":", pos) + 1);
		client.sendMessage("NOTICE " + msg);
	}
}