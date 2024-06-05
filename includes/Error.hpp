#ifndef ERROR_HPP
#define ERROR_HPP
#include <string>
#include "../includes/Client.hpp"
#include <sys/socket.h>


void ERR_NOSUCHCHANNEL();
void ERR_NEEDMOREPARAMS();
void ERR_NOTONCHANNEL();
void ERR_CHANOPRIVSNEEDED();
void ERR_USERONCHANNEL();
void sendErrorToClient(const std::string &message, Client &client);

#endif