#include "Client.hpp"

Client::Client() : _fdClient(-1), _corrPass(false)
{
    _nick = "";
	_inChannel = false;
    _username = "";
    _realname = "";
	_asDefNick = false;
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

bool Client::getDefNick()
{
	return this->_asDefNick;
}

void Client::setDefNick()
{
	this->_asDefNick = true;
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

bool    Client::operator==(const Client &other) const
{
    return _fdClient == other._fdClient;
}

std::string Client::getUsername() const
{
    return _username;
}

std::string Client::getRealname() const {
    return _realname;
}

void Client::setUsername(std::string& usr) {
    _username = usr;
}

void Client::setRealname(std::string &rl) {
    _realname = rl;
}

void Client::setNoChannelActive()
{
    _activeChannel = NULL;
    _inChannel = false;
}