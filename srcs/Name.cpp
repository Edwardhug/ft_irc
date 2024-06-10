#include "../includes/Server.hpp"
#include "../includes/Error.hpp"
void    Server::attributeNickName(int fd, const char *buffer)
{
    std::string find;
    find = static_cast<std::string>(buffer);
    std::string newNick = find.substr(find.find("NICK") + 5);
    Client *from;
    try {
        from = &findClientWithFd(fd);
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
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
    for (size_t i = 0; i < _vecClient.size(); i++)
    {
        if (_vecClient[i].getFdClient() == fd)
        {

            _vecClient[i].setNick(newNick);
            std::cout << "The nick is : " << _vecClient[i].getNick() << std::endl;
            return ;
        }
    }
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