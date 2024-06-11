#include "../includes/Channel.hpp"
#include "../includes/ErrorAndReply.hpp"

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
    res += "\r\n";
    return res;
}


void Channel::deleteOperator(std::string target, Client& from)
{
    std::deque<Client*>::iterator it;
    for (it = _operators.begin(); it != _operators.end(); it++)
    {
        if ((*it)->getNick() == target)
        {
            _operators.erase(it);
            std::string notif = ":" + from.getNick() + " MODE " + _name + " -o " + target + "\r\n";
            msgToChannel(notif);
            break ;
        }
    }
}

void    Channel::msgToChannel(std::string msg)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (!servSendMessageToClient(msg, *_clients[i]))
            return;
    }
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
    if (checkOperatorWithName(target))
    {
        return ERR_OPEALREADY(from, target, _name);
    }
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getNick() == target)
        {
            _operators.push_back(_clients[i]);
            std::string notif = ":" + from.getNick() + " MODE " + _name + " +o " + target + "\r\n";
            msgToChannel(notif);
            break ;
        }
    }
}

void    Channel::addModes(char mode, Client& from, std::string target)
{
    if (mode == 'o')
    {
        if (target.empty())
        {
            return ERR_NEEDMOREPARAMSCHANNEL(from, _name, "MODE +o");
        }
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
        return ERR_MODEALREADY(from, _name, mode, "activated");
    }
    else
    {
        if (mode == 'k')
        {
            if (target.empty())
            {
                ERR_NEEDMOREPARAMSCHANNEL(from, _name, "MODE +k");
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
                ERR_NEEDMOREPARAMSCHANNEL(from, _name, "MODE +l");
                return;
            }
            if (!strIsDigit(target))
            {
                return ERR_NEEDMOREPARAMSCHANNEL(from, _name, "MODE +l");
            }
            _maxClient = ft_atoi(target.c_str());
        }
        _modes[mode] = true;
        std::string notif = ":" + from.getNick() + " MODE " + _name + " +" + mode;
        if (!target.empty())
        {
            notif += " " + target;
        }
        notif += "\r\n";
        msgToChannel(notif);
    }
}

void Channel::delModes(char mode, Client& from, std::string target)
{
    std::map<char, bool>::iterator it;
    it = _modes.find(mode);
    if (mode == 'o')
    {
        if (target.empty())
        {
            return ERR_NEEDMOREPARAMSCHANNEL(from, _name, "MODE +o");
        }
        deleteOperator(target, from);
        return ;
    }
    if (!it->second)
    {
        return ERR_MODEALREADY(from, _name, mode, "desactivated");
    }
    else
    {
        it->second = false;
        std::string notif = ":" + from.getNick() + " MODE " + _name + " -" + mode + "\r\n";
        msgToChannel(notif);
    }
}

void Channel::changeMode(char addOrDel, char mode, Client& from, std::string target)
{
    target = removeSpaces(target);
    bool findClientInOperator = checkOperator(from);
    if (!findClientInOperator)
    {
        return ERR_CHANOPRIVSNEEDED(from, _name);
    }
    if (addOrDel == '+')
    {
        addModes(mode, from, target);
    }
    else
       delModes(mode, from, target);
}

bool    Server::sendActivatedModes()
{
    
}

void    Server::splitForMode(const std::string &buff, int fdSender)
{
    std::string target;
    std::string data = buff.substr(buff.find("MODE") + 5);
    std::deque<std::string> datas = split(data, ' ');
    Client *from;
    std::string toRet;

    if (datas.size() == 1) { // exporter dans une fonction
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
        Client* c;
        try
        {
            c = &findClientWithFd(fdSender);
        }
        catch (std::runtime_error& e)
        {
            std::cout << e.what() << std::endl;
            return;
        }
        servSendMessageToClient(toRet, *c);
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
    if (datas.size() == 3)
    {
        target = datas[2];
    }
    std::string channel = datas[0];
    std::string what = datas[1];
    if (!from->getPass())
        errorPassword(*from);
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