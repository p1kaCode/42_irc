/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:22:55 by lmorel            #+#    #+#             */
/*   Updated: 2024/03/20 16:47:35 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }
    int i = 0;
    while (argv[1][i])
    {
        if (!isdigit(argv[1][i]))
        {
            std::cerr << "Bad port: " << argv[1] << ", please use a numeric port." << std::endl;
            return 1;
        }
        i++;   
    }

    int port = atoi(argv[1]);
    std::string password = argv[2];

    Server server(port, password);
    server.run();

    return 0;
}
