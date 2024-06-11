#include <iostream>
#include <csignal>
#include <string>
#include "../includes/lib.hpp"
#include "../includes/Server.hpp"

bool g_signal = false;

bool validArg(char **av)
{
	if (ft_atoi(av[1]) < 1024 || ft_atoi(av[1]) > 49151)
		return false;
	return true;
}

int	main(int ac, char **av)
{
	if (ac != 3  || validArg(av) == false)
	{
		std::cerr << "Usage: " << av[0] << " <port number> <password>" << std::endl;
		return (1);
	}

	// to close fds, need to create the serv object here
	Server serv(ft_atoi(av[1]), av[2]);

	std::cout << "Server started\n";
	try {
		std::signal(SIGINT, signalHandler);
		std::signal(SIGQUIT, signalHandler);
		serv.servInit();
		serv.servLoop();
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	serv.closeFds();
	std::cout << "Server closed\n";
	(void) serv;
	return (0);
}
