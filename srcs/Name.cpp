#include "../includes/Server.hpp"
#include "../includes/ErrorAndReply.hpp"
void    Server::attributeNickName(int fd, std::string& buff)
{
    std::string newNick = buff.substr(buff.find("NICK") + 5);
    Client *from;
    try {
        from = &findClientWithFd(fd);
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        return ;
    }
    if (newNick.empty())
    {
        return ERR_NONICKNAMEGIVEN(*from);
    }
    if (!verifyNick(newNick))
    {
        return ERR_ERRONEUSNICKNAME(*from, newNick);
    }
    if (nickAlreadyExist(newNick))
    {
        return ERR_NICKNAMEINUSE(*from, newNick);
    }
    from->setNick(newNick);
}

bool    Server::verifyNick(std::string& nick)
{
    if (isdigit(nick[0]) || nick.find(' ') != std::string::npos || nick.find('#') != std::string::npos || nick.find('&') != std::string::npos || nick.find(":") != std::string::npos)
    {
        return false;
    }
    return true;
}

bool    Server::nickAlreadyExist(std::string& nick)
{
    for (size_t i = 0; i < _vecClient.size(); i++)
    {
        if (_vecClient[i].getNick() == nick)
            return true;
    }
    return false;
}

void    Server::setUsername(int fdSender, std::string& buff)
{
    buff = buff.substr(buff.find("USER") + 5);
    std::deque<std::string>datas = splitBuffer(buff, ' ');
    Client *from;
    try
    {
        from = &findClientWithFd(fdSender);
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        return ;
    }
    if (datas.size() < 4 || datas[0].empty() || datas[3].empty())
    {
        return ERR_NEEDMOREPARAMS(*from, "USER");
    }
    if (!from->getRealname().empty() && !from->getUsername().empty())
    {
        return ERR_ALREADYREGISTERED(*from);
    }
    if (datas[1] != "0" && datas[2] != "*")
        return;
    from->setUsername(datas[0]);
    from->setRealname(datas[3]);
}