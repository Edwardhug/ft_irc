#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <poll.h>
#include "Client.hpp"
#include <netinet/in.h>
#include <exception>

class Server 
{
    private:
    int _port;
	std::string _password;
    int _serverSocketFd;
    static bool _signal;
    std::vector<Client> _vecClient;
    std::vector<pollfd> _vecPollFd;


    public:
    Server(int port, std::string password);
    ~Server();
    // Server(const Server &s);
    // Server& operator=(const Server &s);
    void setPort(int port);
    void setServerSocketFd(int fd);
    void addClient(const Client &client);
    void addPollFd(const pollfd &fd);
	void servInit();
};

#endif