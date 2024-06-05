#include "../includes/Server.hpp"
#include "../includes/Error.hpp"
void Server::inviteClient(std::string &buff, int fdSender)
{
    std::string data = buff.substr(buff.find("INVITE") + 7);
    Client from = findClientWithFd(fdSender);
    std::vector<std::string> datas = split(data, ' ');
    if (datas.size() < 2)
    {
        return ERR_NEEDMOREPARAMS(from);
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
        return ERR_NOSUCHCHANNEL(from, channel);
    }
    if (!chToFind->clientInChannel(from))
    {
        return ERR_NOTONCHANNEL(from, channel);
    }
    if (chToFind->checkPerm('i') && !chToFind->checkOperator(from))
    {
        return ERR_CHANOPRIVSNEEDED(from, channel);
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
        return ERR_WASNOSUCHNICK(from, newClientNick);
    }
    if (chToFind->clientInChannel(*newClient))
    {
        return ERR_USERONCHANNEL(from, newClientNick, channel);
    }
    chToFind->addClientInvited(newClient);
    // TODO : TROUVER COMMENT FIX, message pas complet
    std::string rlpInviting = ":server 341 " + from.getNick() + " " + newClientNick + " " + channel + "\r\n";
    servSendMessageToClient(rlpInviting, from);
    //TODO : RPL_INVITING
}

