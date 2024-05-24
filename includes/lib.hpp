#ifndef LIB_HPP
# define LIB_HPP
#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <sstream>
	# define BUFFER_SIZE 1024

	extern bool g_signal;

	void	signalHandler(int signum);
	int		ft_atoi(const char* str);
    std::vector<std::string> split(std::string &buffer, char delimiter);

#endif