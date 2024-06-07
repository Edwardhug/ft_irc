#include "../includes/Channel.hpp"
#include "../includes/Error.hpp"

std::string getModesActivate(Channel chan)
{
    std::map<char, bool> modes = chan.getModes();
    unsigned int maxClient = chan.getMaxClient();
    std::map<char, bool>::iterator it;
    std::string res = ":Serveur NOTICE " + chan.getName() + " Active modes are :+";
    for (it = modes.begin(); it != modes.end(); it++)
    {
        if (it->second)
            res += it->first;
    }
    if (modes['l'])
    {
        std::ostringstream oss;
        oss << maxClient;
        res += " " + oss.str();
    }
    res += "\r\n"; //todo faudra voir avec sylvain si c'est bien ca qu'il faut renvoyer
    return res;
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

void    Channel::channelMsg(std::string msg, int fdSender)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
            send(_clients[i]->getFdClient(), msg.c_str(), msg.size(), 0);
    }
    (void)fdSender;
}

std::string removeSpaces(std::string str)
{
    std::string res;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] != ' ')
            res += str[i];
    }
    return res;
}

void Channel::addOperator(std::string target, Client& from)
{
    target = removeSpaces(target);
    if (checkOperator(from))
    {
        std::cout << "already operator" << std::endl;
        return ;
    }
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getNick() == target)
        {
            _operators.push_back(_clients[i]);
            std::string notif = ":" + from.getNick() + " MODE " + _name + " +o " + target + "\r\n";
            channelMsg(notif, from.getFdClient());
            break ;
        }
    }
}

void    Channel::addModes(char mode, Client& from, std::string target)
{
    if (mode == 'o' && !target.empty())
    {
        addOperator(target, from);
        return ;
    }
    std::map<char, bool>::iterator it;
    it = _modes.find(mode);
    if (it == _modes.end())
    {
        ERR_NEEDMOREPARAMS(from, "MODE"); //todo mettre le code d'erreur de sylvain
        return;
    }
    if (it->second) {
        std::cout << _name << ": " << "mode " << mode << " is already activate." << std::endl; //Envoyer message au client
        return;
    }
    else
    {
        _modes[mode] = true;
        if (mode == 'k')
        {
            if (target.empty())
            {
                ERR_NEEDMOREPARAMS(from, "MODE");
                return;
            }
            if (target.find(' ') != std::string::npos)
            {
                ERR_BADCHANNELKEY(from, _name);
                return ;
            }
            _password = target;
        }
        if (mode == 'l')
        {
            if (target.empty())
            {
                ERR_NEEDMOREPARAMS(from, "MODE");
                return;
            }
            // TODO: Gestion d'erreur pas numero
            _maxClient = ft_atoi(target.c_str());
        }
        std::string notif = ":" + from.getNick() + " MODE " + _name + " +" + mode;
        if (!target.empty())
        {
            notif += " " + target;
        }
        notif += "\r\n";
        channelMsg(notif, from.getFdClient());
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
        channelMsg(notif, from.getFdClient());
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
    Client *from;
    std::string toRet;

    if (datas.size() == 1) {
        Channel chan;
        try {
            chan = findChannelWithName(datas[0]);
        }
        catch (std::runtime_error& e)
        {
            std::cerr << e.what() << std::endl;
            return;
        }
        toRet = getModesActivate(chan);
        std::cout << GREEN << toRet << RESET << std::endl;
        send(fdSender, toRet.c_str(), toRet.size(), 0);
        return ;
    }
    try {
        from = &findClientWithFd(fdSender);
    }
    catch (std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }
    if (datas.size() < 2)
    {
        ERR_NEEDMOREPARAMS(*from, "MODE");
        return ;
    }
    if (datas.size() == 3)
    {
        target = datas[2];
    }
    std::string channel = datas[0];
    std::string what = datas[1];
    if (!from->getPass())
        errorPassword(*from);
    if (what.find('-') == std::string::npos && what.find('+') == std::string::npos)
    {
        ERR_NEEDMOREPARAMS(*from, "MODE");
        return ;
    }
    for (size_t i = 0; i < this->_vecChannel.size(); i++)
    {
        if (channel == _vecChannel[i].getName())
        {
            Channel& chan = _vecChannel[i];
            char addOrDel = what[0];
            char mode = what[1];
            if (target.empty())
            {
                chan.changeMode(addOrDel, mode, *from, "");
            }
            else
                chan.changeMode(addOrDel, mode, *from, target);
            break ;
        }
    }
} 