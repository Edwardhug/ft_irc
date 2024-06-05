#include "../includes/Server.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "../includes/lib.hpp"
#include "../includes/Error.hpp"
#include <cstdio>
#include <cstring>

void	Server::sendConfirmation(std::vector<std::string> data, Client &client) {
	std::string joinMsg = ":" + client.getNick() + " JOIN " + data[1] + "\r\n";
    send(client.getFdClient(), joinMsg.c_str(), joinMsg.length(), 0);
    // Envoyer le sujet du canal (332)
    std::string topicMsg = ":server 332 " + client.getNick() + " " + data[1] + " :Welcome to the new channel " + data[1] + "\r\n";
    send(client.getFdClient(), topicMsg.c_str(), topicMsg.length(), 0);
    // Envoyer la liste des utilisateurs (353)
    std::string namesMsg = ":server 353 " + client.getNick() + " = " + data[1] + " :" + client.getNick() + "\r\n";
    send(client.getFdClient(), namesMsg.c_str(), namesMsg.length(), 0);
    // Envoyer la fin de la liste des utilisateurs (366)
    std::string endOfNamesMsg = ":server 366 " + client.getNick() + " " + data[1] + " :End of /NAMES list\r\n";
    send(client.getFdClient(), endOfNamesMsg.c_str(), endOfNamesMsg.length(), 0);
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

Channel &Server::findChannelWithName(std::string name) {
	size_t i;
    for (i = 0; i < _vecChannel.size(); i++)
    {
        if (_vecChannel[i].getName() == name) {
			return _vecChannel[i];
		}
    }
	return _vecChannel[i]; // arrive jamais ici parce qu'on fait les check avant
}

bool isValidName(std::string name) {
    if (name[5] != '#') {
        std::cout << RED << "Error: Invalid channel name. Channel name must start with #." << RESET << std::endl;
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
	Client client;
    try
    {
        client = findClientWithFd(fdSender);
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        return ;
    }
    data = split(buff, ' ');
    if (data.size() < 2)
    {
        return ERR_NEEDMOREPARAMS(client);
    }
	if (data.size() >= 2 && channelExist(data[1]) == false)
    {
		Channel newChannel(data[1], &client);
		addChannel(newChannel);
		client.changeChannelBool(true);
		client.setChannel(findChannelWithName(data[1]));
		sendConfirmation(data, client);
	}
	else if (data.size() >= 2 && channelExist(data[1]) == true) {
        Channel& channel = findChannelWithName(data[1]);
        channel.checkModesForJoin(client);
        if (channel.checkPerm('k'))
        {
            if (data.size() < 3)
                return ERR_BADCHANNELKEY(client, data[1]);
            if (data[2] != channel.getPass())
                return ERR_BADCHANNELKEY(client, data[1]);
        }
		addClientToChannel(data[1], client);
		client.changeChannelBool(true);
		client.setChannel(findChannelWithName(data[1]));
		sendConfirmation(data, client);
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
    Client &senderClient = findClientWithFd(fdSender);
	char *message = getMessage(buffer);
    std::string channelName = findChannelName(buffer);
    Channel channel = findChannelWithName(channelName);
	std::string fullMessage = ":" + senderClient.getNick() + "!" + senderClient.getNick() + "@server PRIVMSG " + channelName + " :" + message + "\r\n";
    std::vector<Client*> vecClient = channel.getVecClient();
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
