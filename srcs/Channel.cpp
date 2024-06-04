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

void Channel::deleteOperator(std::string target)
{
    std::vector<Client*>::iterator it;
    for (it = _operators.begin(); it != _operators.end(); it++)
    {
        if ((*it)->getNick() == target)
        {
            _operators.erase(it);
            break ;
        }
    }
}

void Channel::addOperator(std::string target)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getNick() == target)
        {
            _operators.push_back(_clients[i]);
            break ;
        }
    }
}

void Channel::changeMode(char addOrDel, char mode, Client& from, std::string target)
{
    std::map<char, bool>::iterator it;
    it = _modes.find(mode);
    bool findClientInOperator = false;
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (*_operators[i] == from)
            findClientInOperator = true;
    }
    if (!findClientInOperator)
    {
        std::cout << "Client is not an operator" << std::endl;
        return ;
    }
    if (addOrDel == '+')
    {
        if (mode == 'o' && !target.empty())
        {
            addOperator(target);
        }
        if (it->second)
            std::cout << _name << ": " << "mode " << mode << " is already activate." << std::endl; //Envoyer message au client
        else
        {
            it->second = true;
            if (mode == 'k')
            {
                if (target.empty())
                {
                    //!"<client> <target chan/user> <mode char> <parameter> :<description>"
                    return;
                }
                if (target.find(' ') != std::string::npos)
                {
                    //!"<client> <target chan/user> <mode char> <parameter> :<description>"
                    return ;
                }
                _password = target;
            }
            if (mode == 'l')
            {
                if (target.empty())
                {
                    // ! "<client> <target chan/user> <mode char> <parameter> :<description>"
                }
                // TODO Gestion d'erreur pas numero
                _maxClient = ft_atoi(target.c_str());
            }
            std::string notif = ":" + from.getNick() + " MODE " + _name + " " + addOrDel + mode + "\r\n";
            //channelMsg(notif.c_str(), from.getFdClient());
        }
    }
    else
    {
        if (mode == 'o' && !target.empty())
        {
            deleteOperator(target);
        }
        if (!it->second)
        {
            std::cout << _name << ": " << "mode " << mode << " is already desactivate." << std::endl;
        }
        else
        {
            it->second = false;
            std::string notif = ":" + from.getNick() + " MODE " + _name + " " + addOrDel + mode + "\r\n";
//            channelMsg(notif.c_str(), from.getFdClient());
        }
    }
}
void	Channel::addClient(Client *newClient) {
	_clients.push_back(newClient);
}


std::vector<Client*> Channel::getVecClient() {
	return (_clients);
}

