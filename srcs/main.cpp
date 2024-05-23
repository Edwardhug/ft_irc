#include <iostream>
#include <csignal>
#include "../includes/lib.hpp"

int	main(int ac, char **av)
{
	// to close fds, need to create the serv object here
	if (ac != 3)
	{
		std::cerr << "Usage: " << av[0] << " <port number> <password>" << std::endl;
		return (1);
	}
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
	return (0);
}