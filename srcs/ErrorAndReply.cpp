#include "../includes/ErrorAndReply.hpp"

void ERR_NOSUCHCHANNEL(Client& client, std::string channel)
{
    std::string err = ":server 403 " + client.getNick() + " " + channel + " :No such channel\r\n";
    servSendMessageToClient(err, client);
}

void ERR_NEEDMOREPARAMS(Client& client, std::string fx)
{
    std::string err = ":server 461 " + client.getNick() + " " + fx + " :Not enough parameters\r\n";
    servSendMessageToClient(err, client);
}

void ERR_NEEDMOREPARAMSCHANNEL(Client& client, std::string channel, std::string fx)
{
    std::string err = ":server 461 " + client.getNick() + " " + channel + " " + fx + " :Not enough parameters\r\n";
    servSendMessageToClient(err, client);
}

void ERR_NOTONCHANNEL(Client& client, std::string channel)
{
    std::string err = ":server 442 " + client.getNick() + " " + channel + " :You're not on that channel\r\n";
    servSendMessageToClient(err, client);
}

void ERR_CHANOPRIVSNEEDED(Client& client, std::string channel)
{
    std::string err = ":server 482 " + client.getNick() + " " + channel + " :You're not channel operator\r\n";
    servSendMessageToClient(err, client);
}

void ERR_USERONCHANNEL(Client& client, std::string nick, std::string channel)
{
    std::string err = ":server 443 " + client.getNick() + " " + nick + " " + channel + " :Is already on channel\r\n";
    servSendMessageToClient(err, client);
}

void ERR_WASNOSUCHNICK(Client& client, std::string nick)
{
    std::string err = ":server 406 " + client.getNick() + " " + nick + " :There was no such nickname\r\n";
    servSendMessageToClient(err, client);
}

void ERR_PASSWDISMATCH(Client& client)
{
    std::string err = ":server 464 " + client.getNick() + " :Password incorrect\r\n";
    servSendMessageToClient(err, client);
}

void ERR_CHANNELISFULL(Client& client, std::string channel)
{
    std::string err = ":server 471 " + client.getNick() + " " + channel + " :Cannot join channel (+l)\r\n";
    servSendMessageToClient(err, client);
}

void ERR_INVITEONLYCHAN(Client& client, std::string channel)
{
    std::string err = ":server 473 " + client.getNick() + " " + channel + " :Cannot join channel (+i)\r\n";
    servSendMessageToClient(err, client);
}

void ERR_BADCHANNELKEY(Client& client, std::string channel)
{
    std::string err = ":server 475 " + client.getNick() + " " + channel + " :Cannot join channel (+k)\r\n";
    servSendMessageToClient(err, client);
}

void RPL_INVITING(Client& from, Client& to, std::string channel)
{
    std::string reply = ":server 341 " + from.getNick() + " " + to.getNick() + " " + channel + "\r\n";
    servSendMessageToClient(reply, from);
    servSendMessageToClient(reply, to);
}

void RPL_CHANNELMODEIS(Client& client, std::string channel, std::string modes)
{
    std::string reply = ":server 324 " + client.getNick() + " " + channel + " " + modes;
    servSendMessageToClient(reply, client);
}

void ERR_MODEALREADY(Client& client, std::string channel, char mode, std::string thisErr)
{
    std::string err = ":Server NOTICE " + channel + " :MODE " + mode + " is already " + thisErr + "\r\n";
    servSendMessageToClient(err, client);
}

void ERR_OPEALREADY(Client& client, std::string& to, std::string channel)
{
    std::string err = ":Server NOTICE " + channel + " :MODE o " + to + " is already operator\r\n";
    servSendMessageToClient(err, client);
}

void ERR_ALREADYREGISTERED(Client& client)
{
    std::string err = ":server 462 " + client.getNick() + " :You may not reregister\r\n";
    servSendMessageToClient(err, client);
}

void ERR_NONICKNAMEGIVEN(Client& client)
{
    std::string err = ":server 431 " + client.getNick() + " :No nickname given\r\n";
    servSendMessageToClient(err, client);
}

void ERR_NICKNAMEINUSE(Client& client, std::string& nick)
{
    std::string err = ":server 433 " + client.getNick() + " " + nick + " :Nickname is already in use\r\n";
    servSendMessageToClient(err, client);
}

void ERR_ERRONEUSNICKNAME(Client& client, std::string& nick)
{
    std::string err = ":server 432 " + client.getNick() + " " + nick + " :Erroneus nickname\r\n";
    servSendMessageToClient(err, client);;
}

void RPL_AWAY(Client& client, std::string &nick, std::string message)
{
    std::string rpl = ":server 301 " + client.getNick() + " " + nick + " :" + message + "\r\n";
    servSendMessageToClient(rpl, client);
}

void ERR_NOSUCHNICK(Client& client, std::string &nick)
{
    std::string err = ":server 401 " + client.getNick() + " " + nick + " :No such nick\r\n";
    servSendMessageToClient(err, client);
}

void ERR_NOTEXTTOSEND(Client &client)
{
    std::string err = ":server 412 " + client.getNick() + " :No text to send\r\n";
    servSendMessageToClient(err, client);
}

void ERR_USERNOTINCHANNEL(Client& client, std::string &nick, std::string channel)
{
    std::string err = ":server 441 " + client.getNick() + " " + nick + " " + channel + " :They aren't on that channel\r\n";
    servSendMessageToClient(err, client);
}

void RPL_PRVMSG(Client& clientTo, std::string from, std::string to, std::string message)
{
	std::string rpl = ":" + from + " PRIVMSG " + to + " :" + message + "\r\n";
	servSendMessageToClient(rpl, clientTo);
}