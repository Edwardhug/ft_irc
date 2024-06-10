#ifndef LIB_HPP
# define LIB_HPP
#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <sstream>
#include "../includes/Client.hpp"
#include <cstdio>
    class Client;

	# define BUFFER_SIZE 1024
	# define RED "\033[1;31m"
	# define GREEN "\033[1;32m"
	# define YELLOW "\033[1;33m"
	# define BLUE "\033[1;34m"
	# define RESET "\033[0m"

	extern bool g_signal;

	void	signalHandler(int signum);
	int		ft_atoi(const char* str);
    std::vector<std::string> split(const std::string &buffer, char delimiter);
    std::vector<std::string> splitBuffer(const std::string &buffer, char delimiter);
	std::vector<std::string> splitAllSpaces(std::string &str);
    bool strIsDigit(std::string &str);
    bool servSendMessageToClient(const std::string &message, Client &client);

#endif