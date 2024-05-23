#include "Client.hpp"

Client::Client()
{}

Client::~Client() {}

void Client::setFdClient(int fd)
{
    fdClient = fd;
}

void Client::setIpAddr(const std::string &ipAddr)
{
    this->ipAddrClient = ipAddr;
}

int Client::getFdClient()
{
    return (this->fdClient);
}

std::string Client::getIpAddr()
{
    return (this->ipAddrClient);
}