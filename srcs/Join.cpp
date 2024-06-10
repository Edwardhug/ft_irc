#include "../includes/Server.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "../includes/lib.hpp"
#include "../includes/Error.hpp"
#include <cstdio>
#include <cstring>
#include "../includes/Error.hpp"

void    Server::sendMessageToChannel(std::string channelName, std::string message) {
    Channel &channel = findChannelWithName(channelName);
    std::vector<Client*> vecClient = channel.getVecClient();
    for (size_t i = 0; i < vecClient.size(); ++i) {
        int clientFd = vecClient[i]->getFdClient();
        if (clientFd != -1) {
            ssize_t bytesSent = send(clientFd, message.c_str(), message.length(), 0);
            if (bytesSent == -1) {
                std::cerr << RED << "Error when sending data to client " << vecClient[i]->getNick() << ": ";
                perror("");
                std::cerr << RESET;
            }
        }
    }
}

void	Server::sendConfirmation(std::vector<std::string> data, Client &client) {
	std::string joinMsg = ":" + client.getNick() + " JOIN " + data[1] + "\r\n"; // envoie au nouveau client le message de join et ajoute le client au channel
    sendMessageToChannel(data[1], joinMsg);
    std::string topicMsg = ":server 332 " + client.getNick() + " " + data[1] + " :Welcome to the new channel " + data[1] + "\r\n";
    ssize_t bytesSent = send(client.getFdClient(), topicMsg.c_str(), topicMsg.length(), 0);
    if (bytesSent == -1)
    {
        std::cerr << RED << "Error when sending data to client " << client.getNick() << ": ";
        perror("");
        std::cerr << RESET;
    }
    std::string nameList = ":server 353 " + client.getNick() + " = " + data[1] + " :"; // envoie au client la liste des noms des clients dans le channel
    for (size_t i = 0; i < _vecChannel.size(); ++i) {
        if (_vecChannel[i].getName() == data[1]) {
            std::vector<Client*> vecClient = _vecChannel[i].getVecClient();
            for (size_t j = 0; j < vecClient.size(); ++j) {
                if (_vecChannel[i].checkOperator(*vecClient[j]) == true)
                    nameList += "@" + vecClient[j]->getNick() + " ";
                else
                    nameList += vecClient[j]->getNick() + " ";
               // nameList += vecClient[j]->getNick() + " ";
            }
            nameList += "\r\n"; // ? peut etre un soucis avec les nom qui apparaisse 2 fois mais ca a pas l'air d'etre un probleme
            bytesSent = send(client.getFdClient(), nameList.c_str(), nameList.length(), 0);
            if (bytesSent == -1)
            {
                std::cerr << RED << "Error when sending data to client " << client.getNick() << ": ";
                perror("");
                std::cerr << RESET;
            }
            break;
        }
    }
    std::string endOfNamesMsg = ":server 366 " + client.getNick() + " " + data[1] + " :End of /NAMES list\r\n";
    bytesSent = send(client.getFdClient(), endOfNamesMsg.c_str(), endOfNamesMsg.length(), 0);
    if (bytesSent == -1) {
        std::cerr << RED << "Error when sending data to client " << client.getNick() << ": ";
        perror("");
        std::cerr << RESET;
    }
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
	std::vector<std::string> data;
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
		Channel newChannel(data[1], client);
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
            if (data.size() < 3)
                return ERR_BADCHANNELKEY(*client, data[1]);
            if (data[2] != channel.getPass())
                return ERR_BADCHANNELKEY(*client, data[1]);
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
    std::vector<Client*> vecClient = channel.getVecClient();
    if (channel.clientInChannel(*senderClient) == false) {
        return ERR_NOTONCHANNEL(*senderClient, channelName);
    }
    for (size_t i = 0; i < vecClient.size(); ++i) {
        int clientFd = vecClient[i]->getFdClient();
        if (clientFd != fdSender) {
            ssize_t bytesSent = send(clientFd, fullMessage.c_str(), fullMessage.length(), 0);
            if (bytesSent == -1) {
                std::cerr << RED << "Error when sending data to client " << vecClient[i]->getNick() << ": ";
                perror("");
                std::cerr << RESET;
            }
        }
    }
}
