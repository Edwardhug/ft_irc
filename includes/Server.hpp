#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <poll.h>
#include <netinet/in.h>
#include <exception>
#include <iostream>
#include <sys/socket.h>
#include "Client.hpp"
#include "Channel.hpp"
class Channel;
class Client;
class Server 
{
    private:
    int _port;
    std::string _password;
    int _serverSocketFd;
    static bool _signal;
    std::vector<Client> _vecClient;
    std::vector<pollfd> _vecPollFd;	// peut etre besoin de mettre <struct pollfd>
	std::vector<Channel> _vecChannel;

public:
    Server(int port, std::string password);
    ~Server();
    // Server(const Server &s);
    // Server& operator=(const Server &s);
    void setPort(int port);
    void setServerSocketFd(int fd);
    void addClient(const Client &client);
    void addPollFd(const pollfd &fd);
	void addChannel(const Channel &channel);
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
    void operatorCanals(const char *buffer, int fdSender);
    void sendmsg(const std::string &from, const std::string &to, const std::string& message);
    void attributeNickName(int fd, std::string &buff);
    void splitForPrivMsg(const std::string &buff, int fdSender);
    void splitForMode(const std::string &buff, int fdSender);
	void splitForJoin(std::string buff, int fdSender);
    Client& findClientWithNick(const std::string &nick);
    Client& findClientWithFd(int fd);
	bool	channelExist(std::string name);
	void    addClientToChannel(std::string nameChannel, Client &client);
	Channel &findChannelWithName(std::string name);
	void	channelMsg(char *buffer, int fdSender);
	void	sendConfirmation(std::vector<std::string> data, Client &client);
	void    checkPass(const std::string &buff, int fdClient);
    void errorPassword(Client& client);
    char *getMessage(char *buffer);
    void splitForTopic(std::string buff, int fdSender);
    void inviteClient(std::string &buff, int fdSender);
    void kickClient(std::string &buff, int fdSender);
    void sendMessageToChannel(std::string buff, std::string message);
    bool nickAlreadyExist(std::string &nick);
    bool verifyNick(std::string &nick);
    void setUsername(int fdSender, std::string& buff);
};

#endif