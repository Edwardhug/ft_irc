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
void sendErrorToClient(const std::string &message, Client &client);
void ERR_PASSWDISMATCH(Client& client);
void ERR_CHANNELISFULL(Client& client, std::string channel);
void ERR_INVITEONLYCHAN(Client& client, std::string channel);
void ERR_BADCHANNELKEY(Client& client, std::string channel);
void RPL_INVITING(Client& from, Client& to, std::string channel);
#endif