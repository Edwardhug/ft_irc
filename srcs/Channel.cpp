#include "../includes/Channel.hpp"

Channel::Channel() {}

Channel::Channel(std::string name, Client *creator)
{
    this->_name = name;
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

void Channel::displayFDS()
{
    for (size_t i = 0; i < _operators.size(); i++)
    {
        std::cout << RED << _operators[i]->getFdClient() << RESET << std::endl;
    }
}
