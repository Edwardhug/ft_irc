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

}