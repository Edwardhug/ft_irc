#ifndef LIB_HPP
# define LIB_HPP

	# define BUFFER_SIZE 1024

	extern bool g_signal;


	void	signalHandler(int signum);
	int		ft_atoi(const char* str);

#endif