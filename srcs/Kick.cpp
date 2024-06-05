#include "../includes/Server.hpp"
#include "../includes/Error.hpp"

void	Server::kickClient(std::string &buff, int fdSender) {
	std::vector<std::string> data;
	Client &client = findClientWithFd(fdSender);
	data = split(buff, ' ');
	if (data.size() < 3) {
		return ERR_NEEDMOREPARAMS(client);
	}
	if (channelExist(data[1]) == false) {
		return ERR_NOSUCHCHANNEL(client, data[1]);
	}
	data[2] = data[2].erase(data[2].size() - 1);
	Channel &channel = findChannelWithName(data[1]);
	if (channel.checkOperator(client) == false) {
		return ERR_CHANOPRIVSNEEDED(client, channel.getName());
	}
	Client clientToKick;
	try {
		clientToKick = findClientWithNick(data[2]);
	}
	catch (std::exception &e) {
		return ERR_USERONCHANNEL(client, data[2], channel.getName());
	}

	if (channel.clientInChannel(clientToKick) == false) {
		return ERR_USERONCHANNEL(client, data[2], channel.getName());
	}
	channel.removeClient(clientToKick);
	std::string message = ":server KICK " + channel.getName() + " " + clientToKick.getNick() + " :Kicked by " + client.getNick() + "\r\n";
	servSendMessageToClient(message, client);
	servSendMessageToClient(message, clientToKick);
}
