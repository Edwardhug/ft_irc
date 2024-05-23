#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <poll.h>
#include "Client.hpp"

class Server 
{
    private:
    int port;
    int serverSocketFd;
    static bool signal;
    std::vector<Client> vecClient;
    std::vector<pollfd> vecPollFd; 

    public:
    Server();
    ~Server();
    // Server(const Server &s);
    // Server& operator=(const Server &s);
    void setPort(int port);
    void setServerSocketFd(int fd);
    void addClient(const Client &client);
    void addPollFd(const pollfd &fd);
};

#endif