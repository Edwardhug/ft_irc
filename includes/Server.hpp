#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <poll.h>
#include "Client.hpp"
#include <netinet/in.h>
#include <exception>
#include <iostream>
#include <sys/socket.h>

class Server 
{
    private:
    int _port;
	std::string _password;
    int _serverSocketFd;
    static bool _signal;
    std::vector<Client> _vecClient;
    std::vector<pollfd> _vecPollFd;	// peut etre besoin de mettre <struct pollfd>


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
	void servLoop();
    void closeFds();
    void clearClient(int fd);
    void test();
    void clearPollFds();
    void clearClients();
	void addNewClient();
	void readReceivedData(int fd);
	void removeClient(int fd);
    void operatorCanals(char *buffer, int fdSender);
    void sendmsg(const std::string &from, const std::string &to, const std::string& message);
    bool attributeNickName(int fd, char *buffer);
    int findFdWithNick(const std::string &nick);
    std::string findNickWithFd(int fd);
};

#endif