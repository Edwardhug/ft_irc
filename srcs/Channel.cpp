#include "../includes/Channel.hpp"

Channel::Channel(std::string name, Client *creator)
{
    this->_name = name;
    this->_clients.push_back(creator);
    this->_operators.push_back(creator);
}

Channel::~Channel() {}

void Channel::changeMode(char addOrDel, std::string modes)
{
	(void)addOrDel;
	(void)modes;
}
void	Channel::addClient(Client *newClient) {
	_clients.push_back(newClient);
}

std::string	Channel::getName() {
	return (_name);
}

