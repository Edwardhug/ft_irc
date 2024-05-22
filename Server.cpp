#include "Server.hpp"

Server::Server()
{
    serverSocketFd = -1;
}

Server::~Server()
{}

void Server::setPort(int port)
{
    this->port = port;
}

void Server::setServerSocketFd(int fd)
{
    this->serverSocketFd = fd;
}

void Server::addClient(const Client &client)
{
    vecClient.push_back(client);
}

void Server::addPollFd(const pollfd &fd)
{
    vecPollFd.push_back(fd);
}