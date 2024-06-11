#include "../includes/Server.hpp"
#include "../includes/ErrorAndReply.hpp"

void Server::inviteClient(std::string &buff, int fdSender)
{
    std::string data = buff.substr(buff.find("INVITE") + 7);
    Client *from;
    try{
        from = &findClientWithFd(fdSender);
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        return ;
    }
    std::deque<std::string> datas = split(data, ' ');
    if (datas.size() < 2)
    {
        return ERR_NEEDMOREPARAMS(*from, "INVITE");
    }
    Channel *chToFind;
    try
    {
        chToFind = &findChannelWithName(datas[1]);
    }
    catch (std::runtime_error& e)
    {
        std::cout << RED << e.what() << RESET << std::endl;
        return ERR_NOSUCHCHANNEL(*from, datas[1]);
    }
    if (!chToFind->clientInChannel(*from))
    {
        return ERR_NOTONCHANNEL(*from, datas[1]);
    }
    if (chToFind->checkPerm('i') && !chToFind->checkOperator(*from))
    {
        return ERR_CHANOPRIVSNEEDED(*from, datas[1]);
    }
    Client *newClient;
    try
    {
        newClient = &findClientWithNick(datas[0]);
    }
    catch (std::runtime_error& e)
    {
        std::cout << RED << e.what() << RESET << std::endl;
        return ERR_WASNOSUCHNICK(*from, datas[0]);
    }
    if (chToFind->clientInChannel(*newClient))
    {
        return ERR_USERONCHANNEL(*from, datas[0], datas[1]);
    }
    chToFind->addClientInvited(newClient);
    RPL_INVITING(*from, *newClient, datas[1]);
}

