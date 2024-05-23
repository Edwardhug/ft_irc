#include <iostream>
#include <csignal>
#include <cstdlib>

void signalHandler(int signum)
{
	std::cerr << "Interrupt signal (" << signum << ") received.\n";
	exit(signum);
}