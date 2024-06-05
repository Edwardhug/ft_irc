#include "../includes/Error.hpp"

void sendErrorToClient(const std::string &message, Client &client)
{
    send(client.getFdClient(), message.c_str(), message.length(), 0);
}

void ERR_NOSUCHCHANNEL(Client& client, std::string channel)
{
    std::string err = ":server 403 " + client.getNick() + " " + channel + " :No such channel\r\n";
    sendErrorToClient(err, client);
}

void ERR_NEEDMOREPARAMS(Client& client)
{
    std::string err = ":server 461 " + client.getNick() + " INVITE :Not enough parameters";
    sendErrorToClient(err, client);
}

void ERR_NOTONCHANNEL(Client& client, std::string channel)
{
    std::string err = ":server 442 " + client.getNick() + " " + channel + " :You're not on that channel\r\n";
    sendErrorToClient(err, client);
}

void ERR_CHANOPRIVSNEEDED(Client& client, std::string channel)
{
    std::string err = ":server 482 " + client.getNick() + " " + channel + " :You're not channel operator\r\n";
    sendErrorToClient(err, client);
}

void ERR_USERONCHANNEL(Client& client, std::string nick, std::string channel)
{
    std::string err = ":server 443 " + client.getNick() + " " + nick + " " + channel + " :Is already on channel\r\n";
}

void ERR_WASNOSUCHNICK(Client& client, std::string nick)
{
    std::string err = ":server 406 " + client.getNick() + " " + nick + " :There was no such nickname\r\n";
    sendErrorToClient(err, client);
}