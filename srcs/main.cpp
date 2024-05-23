#include <iostream>
#include <csignal>
#include <string>
#include "../includes/lib.hpp"
#include "../includes/Server.hpp"

int	main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Usage: " << av[0] << " <port number> <password>" << std::endl;
		return (1);
	}

	// to close fds, need to create the serv object here
	Server serv(ft_atoi(av[1]), av[2]);

	std::cout << "Server started\n";
	try {
		std::signal(SIGINT, signalHandler);
		std::signal(SIGTERM, signalHandler);
		// need to start the prog here
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		// need to close fds
	}
	std::cout << "Server closed\n";
	(void) serv;
	return (0);
}