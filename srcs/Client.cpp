#include "Client.hpp"

Client::Client()
{}

Client::~Client() {}

void Client::setFdClient(int fd)
{
    _fdClient = fd;
}

void Client::setIpAddr(const std::string &ipAddr)
{
    this->_ipAddrClient = ipAddr;
}

int Client::getFdClient()
{
    return (this->_fdClient);
}

std::string Client::getIpAddr()
{
    return (this->_ipAddrClient);
}