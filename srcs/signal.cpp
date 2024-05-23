#include <iostream>
#include <csignal>
#include <cstdlib>
#include "../includes/lib.hpp"

void signalHandler(int signum)
{
	std::cerr << "Interrupt signal received.\n";
	(void)signum;
	g_signal = true;
}