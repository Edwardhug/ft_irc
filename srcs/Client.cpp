#include "Client.hpp"

Client::Client() : _fdClient(-1), _corrPass(false)
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

int Client::getFdClient() const
{
    return (this->_fdClient);
}

std::string Client::getIpAddr() const
{
    return (this->_ipAddrClient);
}

void Client::setNick(std::string nick)
{
    this->_nick = nick;
}

std::string Client::getNick() const
{
    return (this->_nick);
}

bool Client::getPass() const
{
    return this->_corrPass;
}

void Client::setPass()
{
    this->_corrPass = true;
}