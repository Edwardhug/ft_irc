#include "../includes/Channel.hpp"

Channel::Channel() {}

Channel::Channel(std::string name, Client *creator, Bot bot)
{
    this->_name = name;
	this->_bot = &bot;
    this->_clients.push_back(creator);
    this->_operators.push_back(creator);
    this->_modes.insert(std::pair<char, bool>('i', false));
    this->_modes.insert(std::pair<char, bool>('t', true));
    this->_modes.insert(std::pair<char, bool>('k', false));
    this->_modes.insert(std::pair<char, bool>('l', false));
    this->_maxClient = 0;
}

Channel::~Channel() {}

unsigned int Channel::getMaxClient()
{
    return this->_maxClient;
}

std::map<char, bool> Channel::getModes()
{
    return this->_modes;
}

std::string Channel::getName() const
{
    return this->_name;
}

void	Channel::addClient(Client *newClient) {
	_clients.push_back(newClient);
}

void    Channel::addClientInvited(Client *newClient)
{
    _clientsInvited.push_back(newClient);
}

std::deque<Client*> Channel::getVecClient() {
	return (_clients);
}

void Channel::setTopic(std::string topic)
{
    if (topic.length() > 50)
        topic = topic.substr(0, 50);
    if (topic[0] == ':')
        topic = topic.substr(1, topic.length() - 1);
    if (topic[topic.length()] == ' ')
        topic = topic.substr(0, topic.length() - 1);
    this->_topic = topic;
}

std::string Channel::getTopic()
{
    return this->_topic;
}

bool Channel::checkPerm(char mode)
{
    std::map<char, bool>::iterator it;
    it = _modes.find(mode);
    return it->second;
}

bool Channel::checkOperatorWithName(std::string nick)
{
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i]->getNick() == nick)
            return true;
    }
    return false;
}

bool Channel::checkOperator(Client &client)
{
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (*_operators[i] == client)
            return true;
    }
    return false;
}

bool Channel::clientInChannel(Client &toFind)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (*_clients[i] == toFind)
            return true;
    }
    return false;
}

void    Channel::removeClient(Client &toRemove)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getFdClient() == toRemove.getFdClient())
        {
            _clients.erase(_clients.begin() + i);
            return;
        }
    }
}

bool	Channel::clientInChannelName(std::string& name)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->getNick() == name)
			return true;
	}
	return false;
}

bool	Channel::clientInChannelFd(int fd)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getFdClient() == fd) {
            return true;
        }
    }
    return false;
}

bool Channel::clientIsInvited(Client &client)
{
    for (size_t i = 0; i < _clientsInvited.size(); i++)
    {
        if (*_clientsInvited[i] == client)
            return true;
    }
    return false;
}

std::string Channel::getPass()
{
    return _password;
}

void Channel::removeClientFd(int fd)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getFdClient() == fd)
        {
            _clients.erase(_clients.begin() + i);
            break;
        }
    }
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i]->getFdClient() == fd)
        {
            _operators.erase(_operators.begin() + i);
            break;
        }
    }
}

void Channel::closeChannel()
{
    while (_clients.size() > 0)
    {
        _clients.erase(_clients.end());
    }
}

bool Channel::sendNamesInChannel(Client& client)
{
    int nbOpe = 0;
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getFdClient() == client.getFdClient())
            continue;
        std::string msgQuit = ":" + client.getNick() + " PART " + _name + " :He was the last operator.\r\n";
        if (!servSendMessageToClient(msgQuit, *_clients[i]))
            return true;
        std::string nameList = ":server 353 " + _clients[i]->getNick() + " = " + _name + " :";
        for (size_t j = 0; j < _clients.size(); ++j) {
            if (checkOperator(*_clients[j]) == true) {
                nameList += "@" + _clients[j]->getNick() + " ";
                nbOpe++;
            }
            else
                nameList += _clients[j]->getNick() + " ";
        }
        nameList += "\r\n";
        if (!servSendMessageToClient(nameList, *_clients[i]))
            return true;
        std::string endOfNamesMsg = ":server 366 " + _clients[i]->getNick() + " " + _name + " :End of /NAMES list\r\n";
        if (!servSendMessageToClient(endOfNamesMsg, *_clients[i]))
            return true;
    }
    if (nbOpe == 0)
    {
        std::string noOpe = ":Server NOTICE " + _name + " :Channel closed (No more operator in the channel)\r\n";
        msgToChannel(noOpe);
        return false;
    }
    return true;
}