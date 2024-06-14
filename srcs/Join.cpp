#include "../includes/Server.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "../includes/lib.hpp"
#include "../includes/ErrorAndReply.hpp"
#include <cstdio>
#include <cstring>

void    Server::sendMessageToChannel(std::string channelName, std::string message) {
    Channel &channel = findChannelWithName(channelName);
    channel.msgToChannel(message);
}

void	Server::sendConfirmation(std::deque<std::string> data, Client &client) {
	std::string joinMsg = ":" + client.getNick() + " JOIN " + data[1] + "\r\n";
    sendMessageToChannel(data[1], joinMsg);
    std::string topicMsg = ":server 332 " + client.getNick() + " " + data[1] + " :Welcome to the new channel " + data[1] + "\r\n";
    if (!servSendMessageToClient(topicMsg, client))
        return ;
    std::string nameList = ":server 353 " + client.getNick() + " = " + data[1] + " :";
    for (size_t i = 0; i < _vecChannel.size(); ++i) {
        if (_vecChannel[i].getName() == data[1]) {
            std::deque<Client*> vecClient = _vecChannel[i].getVecClient();
            for (size_t j = 0; j < vecClient.size(); ++j) {
                if (_vecChannel[i].checkOperator(*vecClient[j]) == true)
                    nameList += "@" + vecClient[j]->getNick() + " ";
                else
                    nameList += vecClient[j]->getNick() + " ";
            }
            nameList += "\r\n";
            if (!servSendMessageToClient(nameList, client))
                return ;
            break;
        }
    }
    std::string endOfNamesMsg = ":server 366 " + client.getNick() + " " + data[1] + " :End of /NAMES list\r\n";
    if (!servSendMessageToClient(endOfNamesMsg, client))
        return ;
}

//=====================JOIN============================

bool	Server::channelExist(std::string name) {
	size_t i;
    for (i = 0; i < _vecChannel.size(); i++)
    {
        if (_vecChannel[i].getName() == name)
        {
            return true;
        }
    }
	return false;
}

void Server::addClientToChannel(std::string nameChannel, Client &client) {
	size_t i;
    for (i = 0; i < _vecChannel.size(); i++)
    {
        if (_vecChannel[i].getName() == nameChannel) {
			_vecChannel[i].addClient(&client);
			return ;
		}
    }

}

bool isValidName(std::string name) {
    if (name[5] != '#') {
        return false;
    }
    return true;
}

bool    Channel::checkModesForJoin(Client& client)
{
    std::map<char, bool>::iterator it;
    it = _modes.find('i');
    if (it->second)
    {
        if (!clientIsInvited(client))
        {
            ERR_INVITEONLYCHAN(client, _name);
            return false;
        }
    }
    it = _modes.find('l');
    if (it->second)
    {
        if (_clients.size() == _maxClient)
        {
            ERR_CHANNELISFULL(client, _name);
            return false;
        }
    }
    return true;
}

void	Server::splitForJoin(std::string buff, int fdSender)
{
    if (isValidName(buff) == false) {
        return;
    }
	std::deque<std::string> data;
	Client *client;
    try
    {
        client = &findClientWithFd(fdSender);
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        return ;
    }
    data = split(buff, ' ');
    if (data.size() < 2)
    {
        return ERR_NEEDMOREPARAMS(*client, "JOIN");
    }
	if (data.size() >= 2 && channelExist(data[1]) == false)
    {
//		Bot *bot = new Bot();
		Channel newChannel(data[1], client, NULL);
		addChannel(newChannel);
		client->changeChannelBool(true);
		client->setChannel(findChannelWithName(data[1]));
		sendConfirmation(data, *client);
	}
	else if (data.size() >= 2 && channelExist(data[1]) == true) {
        Channel& channel = findChannelWithName(data[1]);
        if (!channel.checkModesForJoin(*client))
            return ;
        if (channel.checkPerm('k'))
        {
			std::cout << RED << "je passe\n" << RESET;
            if (data.size() < 3)
			{
				std::cout << RED << "ici !!\n" << RESET ;
				return ERR_BADCHANNELKEY(*client, data[1]);
			}
			if (data[2].find(' ') != std::string::npos)
				data[2].erase(data[2].find(' '));
            if (data[2] != channel.getPass())
			{
				for (size_t i = 0; i < data[2].size(); i++)
					std::cout << GREEN << static_cast<int>(data[2][i]) << " ";
				std::cout << '\n' << RESET;
				std::cout << RED << data[2] << " " << data[2].size() << " " << channel.getPass() << std::endl << RESET;
				return ERR_BADCHANNELKEY(*client, data[1]);
			}
        }
		addClientToChannel(data[1], *client);
		client->changeChannelBool(true);
		client->setChannel(findChannelWithName(data[1]));
		sendConfirmation(data, *client);
	}
}
//==============================channel msg=============================

char *Server::getMessage(char *buffer) {
	char *message = strchr(buffer, ':');
	if (message == NULL) {
		return buffer;
	}
	message++;
	return message;
}

std::string findChannelName(char *buffer) {
    std::string channelName = "";
    for (size_t i = 8; buffer[i] != ' '; ++i) {
        channelName += buffer[i];
    }
    return channelName;
}

void Server::channelMsg(char *buffer, int fdSender) {
    Client *senderClient;
    try {
        senderClient = &findClientWithFd(fdSender);
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
        return ;
    }
    char *message = getMessage(buffer);
    std::string channelName = findChannelName(buffer);
    Channel channel = findChannelWithName(channelName);
    std::string fullMessage = ":" + senderClient->getNick() + "!" + senderClient->getNick() + "@server PRIVMSG " + channelName + " :" + message + "\r\n";
    std::deque<Client*> vecClient = channel.getVecClient();
    if (channel.clientInChannel(*senderClient) == false) {
        return ERR_NOTONCHANNEL(*senderClient, channelName);
    }
    for (size_t i = 0; i < vecClient.size(); ++i) {
        int clientFd = vecClient[i]->getFdClient();
        if (clientFd != fdSender) {
            if (!servSendMessageToClient(fullMessage, *vecClient[i]))
                return ;
        }
    }
}
