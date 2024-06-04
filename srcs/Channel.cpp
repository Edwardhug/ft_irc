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
}

Channel::~Channel() {}

std::string Channel::getName() const
{
    return this->_name;
}

void	Channel::addClient(Client *newClient) {
	_clients.push_back(newClient);
}


std::vector<Client*> Channel::getVecClient() {
	return (_clients);
}

void Channel::setTopic(std::string topic)
{
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
