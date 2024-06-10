#include "../includes/Server.hpp"
#include "../includes/Error.hpp"
#include "../includes/lib.hpp"

void	Server::kickClient(std::string &buff, int fdSender) {
	std::vector<std::string> data;
	Client *client;
    try{
        client = &findClientWithFd(fdSender);
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        return ;
    }
	data = split(buff, ' ');
	data = splitAllSpaces(buff);
	if (data.size() < 3) {
		return ERR_NEEDMOREPARAMS(*client, "KICK");
	}
	if (data[1][0] != '#') {
		for (size_t i = 2; i < data.size(); i++) {
			data[i -1] = data[i];
		}
		data[2] = data[2].substr(1);
	}
	if (channelExist(data[1]) == false) {
		return ERR_NOSUCHCHANNEL(*client, data[1]);
	}
	Channel &channel = findChannelWithName(data[1]);
	if (channel.checkOperator(*client) == false) {
		return ERR_CHANOPRIVSNEEDED(*client, channel.getName());
	}
	Client clientToKick;
	try {
		clientToKick = findClientWithNick(data[2]);
	}
	catch (std::runtime_error &e) {
        return ERR_USERNOTINCHANNEL(*client, data[2], channel.getName());
	}
	if (channel.clientInChannel(clientToKick) == false) {
		return ERR_USERONCHANNEL(*client, data[2], channel.getName());
	}
	channel.removeClient(clientToKick);
	std::string message = ":server KICK " + channel.getName() + " " + clientToKick.getNick() + " :Kicked by " + client->getNick() + "\r\n";
	servSendMessageToClient(message, *client);
	servSendMessageToClient(message, clientToKick);
}
 