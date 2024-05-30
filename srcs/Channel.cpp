#include "../includes/Channel.hpp"
#include <iostream>
Channel::Channel(std::string name, Client *creator)
{
    this->_name = name;
    this->_clients.push_back(creator);
    this->_operators.push_back(creator);
    this->_modes.insert(std::pair<char, bool>('i', false));
    this->_modes.insert(std::pair<char, bool>('t', false));
    this->_modes.insert(std::pair<char, bool>('k', false));
    this->_modes.insert(std::pair<char, bool>('o', false));
    this->_modes.insert(std::pair<char, bool>('l', false));
}

Channel::~Channel() {}

std::string Channel::getName() const
{
    return this->_name;
}
void Channel::changeMode(char addOrDel, char mode, Client& from)
{
    std::map<char, bool>::iterator it;
    it = _modes.find(mode);
    if (addOrDel == '+')
    {
        if (it->second)
            std::cout << _name << ": " << "mode " << mode << " is already activate." << std::endl; //Envoyer message au client
        else
        {
            it->second = true;
            std::string notif = ":" + from.getNick() + " MODE " + _name + " " + addOrDel + mode + "\r\n";
            //channelMsg(notif.c_str(), from.getFdClient());
        }
    }
    else
    {
        if (!it->second)
        {
            std::cout << _name << ": " << "mode " << mode << " is already desactivate." << std::endl;
        }
        else
        {
            it->second = false;
            std::string notif = ":" + from.getNick() + " MODE " + _name + " " + addOrDel + mode + "\r\n";
            //channelMsg(notif.c_str(), from.getFdClient());
        }
    }
}
void	Channel::addClient(Client *newClient) {
	_clients.push_back(newClient);
}

