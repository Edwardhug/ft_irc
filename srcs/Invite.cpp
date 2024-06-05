#include "../includes/Server.hpp"

void Server::inviteClient(std::string &buff, int fdSender)
{
    std::string data = buff.substr(buff.find("INVITE") + 7);
    Client from = findClientWithFd(fdSender);
    std::cout << "data : " << data << std::endl;
    std::vector<std::string> datas = split(data, ' ');
    if (datas.size() < 2)
    {
        std::string err = ":server 461 " + from.getNick() + " INVITE :Not enough parameters";
        servSendMessageToClient(err, from);
        return ;
    }
    std::string newClientNick = datas[0];
    std::string channel = datas[1];
    Channel *chToFind = NULL;
    for (size_t i = 0; i < _vecChannel.size(); i++)
    {
        if (_vecChannel[i].getName() == channel)
        {
            chToFind = &_vecChannel[i];
            break;
        }
    }
    if (chToFind == NULL)
    {
        std::string err = ":server 403 " + from.getNick() + " " + channel + " :No such channel\r\n";
        servSendMessageToClient(err, from);
        return;
    }
    if (!chToFind->clientInChannel(from))
    {
        std::string err = ":server 442 " + from.getNick() + " " + channel + " :You're not on that channel\r\n";
        servSendMessageToClient(err, from);
        return ;
    }
    // TODO : Tester plus, ca n'a pas l'air de marcher
    if (chToFind->checkPerm('i') && !chToFind->checkOperator(from))
    {
        std::string err = ":server 482 " + from.getNick() + " " + channel + " :You're not channel operator\r\n";
        servSendMessageToClient(err, from);
        return ;
    }
    Client *newClient = NULL;
    for (size_t i = 0; i < _vecClient.size(); i++)
    {
        if (_vecClient[i].getNick() == newClientNick)
        {
            newClient = &_vecClient[i];
            break;
        }
    }
    if (newClient == NULL)
    {
        std::string err = ":server 403 " + from.getNick() + " " + newClientNick + " :No such client\r\n";
        servSendMessageToClient(err, from);
        return ;
    }
    if (chToFind->clientInChannel(*newClient))
    {
        std::string err = ":server 443 " + from.getNick() + " " + newClientNick + " " + channel + " :Is already on channel\r\n";
        servSendMessageToClient(err, from);
        return ;
    }
    chToFind->addClient(newClient);
    // TODO : TROUVER COMMENT FIX, message pas complet
    std::string rlpInviting = ":server 341 " + from.getNick() + " " + newClientNick + " " + channel + "\r\n";
    servSendMessageToClient(rlpInviting, from);
    //TODO : RPL_INVITING
}

