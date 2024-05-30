#include "Client.hpp"

Client::Client() {
	_inChannel = false;
}

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

void Client::setNick(std::string nick)
{
    this->_nick = nick;
}

std::string Client::getNick()
{
    return (this->_nick);
}

bool	Client::getInChannel() {
	return (_inChannel);
}

void	Client::changeChannelBool() {
	if (_inChannel == true)
		_inChannel = false;
	else if (_inChannel == false)
		_inChannel = true;
}

void	Client::setChannel(Channel &channel) {
	_activeChannel = &channel;
}

Channel Client::getActiveChannel() {
	return *_activeChannel;
}