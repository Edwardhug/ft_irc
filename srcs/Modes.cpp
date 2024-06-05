#include "../includes/Channel.hpp"
#include "../includes/Error.hpp"
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

void    Channel::addModes(char mode, Client& from, std::string target)
{
    std::map<char, bool>::iterator it;
    it = _modes.find(mode);
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
                // TODO: "<client> <target chan/user> <mode char> <parameter> :<description>"
                return;
            }
            if (target.find(' ') != std::string::npos)
            {
                // TODO: "<client> <target chan/user> <mode char> <parameter> :<description>"
                return ;
            }
            _password = target;
        }
        if (mode == 'l')
        {
            if (target.empty())
            {
                // TODO: "<client> <target chan/user> <mode char> <parameter> :<description>"
            }
            // TODO: Gestion d'erreur pas numero
            _maxClient = ft_atoi(target.c_str());
        }
        std::string notif = ":" + from.getNick() + " MODE " + _name + " +" + mode + "\r\n";
        //channelMsg(notif.c_str(), from.getFdClient());
    }
}

void Channel::delModes(char mode, Client& from, std::string target)
{
    std::map<char, bool>::iterator it;
    it = _modes.find(mode);
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
        std::string notif = ":" + from.getNick() + " MODE " + _name + " -" + mode + "\r\n";
//            channelMsg(notif.c_str(), from.getFdClient());
    }
}

void Channel::changeMode(char addOrDel, char mode, Client& from, std::string target)
{
    bool findClientInOperator = checkOperator(from);
    if (!findClientInOperator)
    {
        std::cout << "Client is not an operator" << std::endl;
        return ;
    }
    if (addOrDel == '+')
    {
        addModes(mode, from, target);
    }
    else
       delModes(mode, from, target);
}

void    Server::splitForMode(const std::string &buff, int fdSender)
{
    std::string target;
    std::string data = buff.substr(buff.find("MODE") + 5); // TODO : Split et enlever le premier
    std::vector<std::string> datas = split(data, ' ');
    Client from;
    try {
        from = findClientWithFd(fdSender);
    }
    catch (std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }
    if (datas.size() < 2)
    {
        return ERR_NEEDMOREPARAMS(from);
    }
    if (datas.size() == 3)
    {
        target = datas[2];
    }
    std::string channel = datas[0];
    std::string what = datas[1];

    if (!from.getPass())
        errorPassword(from);
    if (what.find('-') == std::string::npos && what.find('+') == std::string::npos)
    {
        std::cerr << "Add - or + before the channel operator";
        return ;
    }
    for (size_t i = 0; i < this->_vecChannel.size(); i++)
    {
        std::cout << GREEN << channel << ". " << _vecChannel[i].getName() << "." << RESET << std::endl;
        if (channel == _vecChannel[i].getName())
        {
            Channel chan = _vecChannel[i];
            char addOrDel = what[0];
            char mode = what[1];
            if (target.empty())
            {
                chan.changeMode(addOrDel, mode, from, "");
            }
            else
                chan.changeMode(addOrDel, mode, from, target);
        }
    }
}