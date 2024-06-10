#include "../includes/lib.hpp"
#include "../includes/Error.hpp"

void    Server::checkPass(const std::string &buff, int fdClient)
{
    std::string rightPass = this->_password;
    size_t passWord = buff.find("PASS ");
    Client *from;
    try
    {
        from = &findClientWithFd(fdClient);
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        return ;
    }
    if (from->getPass())
    {
        return ERR_ALREADYREGISTERED(*from);
    }
    std::string passIn = buff.substr(passWord + 5);
    if (passIn.empty())
    {
        return ERR_NEEDMOREPARAMS(*from, "PASS");
    }
    if (passIn != rightPass)
    {
        return ERR_PASSWDISMATCH(*from);
    }
    else {
        for (size_t i = 0; i < _vecClient.size(); i++)
        {
            if (fdClient == _vecClient[i].getFdClient())
            {
                _vecClient[i].setPass();
            }
        }
    }
}

void Server::errorPassword(Client& client)
{
    servSendMessageToClient(":server 464 " + client.getNick() + " :Password incorrect\r\n", client);
}