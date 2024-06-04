#include "Client.hpp"

Client::Client() : _fdClient(-1), _corrPass(false)
{
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

bool	Client::getInChannel() {
	return (_inChannel);
}

void	Client::changeChannelBool(bool value) {
	if (value == true)
		_inChannel = true;
	else
		_inChannel = false;
}

void	Client::setChannel(Channel &channel) {
	_activeChannel = &channel;
}

Channel Client::getActiveChannel() {
	return *_activeChannel;
}

bool    Client::operator==(const Client &other) const
{
    bool isEqual = true;
    if (this->_fdClient != other._fdClient)
        isEqual = false;
    if (this->_ipAddrClient != other._ipAddrClient)
        isEqual = false;
    if (this->_nick != other._nick)
        isEqual = false;
    if (this->_corrPass != other._corrPass)
        isEqual = false;
    if (this->_inChannel != other._inChannel)
        isEqual = false;
    //faire activeChannel
    return isEqual;
}