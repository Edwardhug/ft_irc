#include "Server.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "../includes/lib.hpp"
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

void	Server::splitForJoin(std::string buff, int fdSender)
{
	std::vector<std::string> data;
	Client &client = findClientWithFd(fdSender);
    data = split(buff, ' ');
	if (data.size() >= 2 && channelExist(data[1]) == false)
    {
		Channel newChannel(data[1], &client);
		addChannel(newChannel);
		client.changeChannelBool();
		client.setChannel(&findChannelWithName(data[1]));
		sendConfirmation(data, client);
	}
	else if (data.size() >= 2 && channelExist(data[1]) == true) {
		addClientToChannel(data[1], client);
		client.changeChannelBool();
		client.setChannel(&findChannelWithName(data[1]));
		sendConfirmation(data, client);
	}
}
//==============================channel msg=============================

char *getMessage(char *buffer) {
	char *message = strchr(buffer, ':');
	if (message == NULL) {
		return buffer;
	}
	message++;
	return message;
}

void Server::channelMsg(char *buffer, int fdSender) {
    Client &senderClient = findClientWithFd(fdSender);
	char *message = getMessage(buffer);
    
    // Récupérer le canal actif du client

    Channel &channel = senderClient.getActiveChannel();
    std::string channelName = channel.getName();

    // Construire le message à envoyer
	std::string fullMessage = ":" + senderClient.getNick() + "!" + senderClient.getNick() + "@server PRIVMSG " + channelName + " :" + message + "\r\n";

    // Déboguer le message complet
    std::cout << "Full message: " << fullMessage << std::endl;

    // Récupérer tous les clients du canal
    std::vector<Client*> vecClient = channel.getVecClient();

    // Envoyer le message à tous les membres du canal sauf l'expéditeur
    for (size_t i = 0; i < vecClient.size(); ++i) {
        int clientFd = vecClient[i]->getFdClient();
        if (clientFd != fdSender) {
            ssize_t bytesSent = send(clientFd, fullMessage.c_str(), fullMessage.length(), 0);
            if (bytesSent == -1) {
                std::cerr << "Error when sending data to client " << vecClient[i]->getNick() << ": ";
                perror("");
            } else {
                std::cout << "Sent message to " << vecClient[i]->getNick() << std::endl;
            }
        }
    }
}


