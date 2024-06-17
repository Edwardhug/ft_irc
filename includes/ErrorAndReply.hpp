#ifndef ERROR_HPP
#define ERROR_HPP
#include <string>
#include "../includes/Client.hpp"
#include <sys/socket.h>


void ERR_NOSUCHCHANNEL(Client& client, std::string channel);
void ERR_NEEDMOREPARAMS(Client& client, std::string fx);
void ERR_NOTONCHANNEL(Client& client, std::string channel);
void ERR_CHANOPRIVSNEEDED(Client& client, std::string channel);
void ERR_USERONCHANNEL(Client& client, std::string nick, std::string channel);
void ERR_WASNOSUCHNICK(Client& client, std::string nick);
void ERR_PASSWDISMATCH(Client& client);
void ERR_CHANNELISFULL(Client& client, std::string channel);
void ERR_INVITEONLYCHAN(Client& client, std::string channel);
void ERR_BADCHANNELKEY(Client& client, std::string channel);
void RPL_INVITING(Client& from, Client& to, std::string channel);
void ERR_NEEDMOREPARAMSCHANNEL(Client& client, std::string channel, std::string fx);
void ERR_MODEALREADY(Client& client, std::string channel, char mode, std::string thisErr);
void ERR_OPEALREADY(Client& client, std::string& to, std::string channel);
void ERR_ALREADYREGISTERED(Client& client);
void ERR_NONICKNAMEGIVEN(Client& client);
void ERR_NICKNAMEINUSE(Client& client, std::string &nick);
void ERR_ERRONEUSNICKNAME(Client& client, std::string& nick);
void RPL_AWAY(Client& client, std::string &nick, std::string message);
void ERR_NOSUCHNICK(Client& client, std::string &nick);
void ERR_NOTEXTTOSEND(Client &client);
void ERR_USERNOTINCHANNEL(Client& client, std::string &nick, std::string channel);
void RPL_PRVMSG(Client& clientTo, std::string from, std::string to, std::string message);
#endif