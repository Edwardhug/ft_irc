#include "../includes/Server.hpp"
#include "../includes/lib.hpp"

void Server::splitForTopic(std::string buff, int fdSender) {
	std::vector<std::string> data;
	Client &client = findClientWithFd(fdSender);
	data = split(buff, ' ');
	if (data.size() >= 3 && channelExist(data[1] + "\r\n") == true) {
		Channel &channel = findChannelWithName(data[1] + "\r\n");
		channel.setTopic(data[2]);
		std::string message = ":" + client.getNick() + " TOPIC " + channel.getName() + " " + channel.getTopic() + "\r\n";
		servSendMessageToClient(message, client);
	}
	else if (data.size() == 2 && channelExist(data[1]) == true) {
		Channel &channel = findChannelWithName(data[1]);
		std::string message = ":" + client.getNick() + " TOPIC " + channel.getName() + " " + channel.getTopic() + "\r\n";
		servSendMessageToClient(message, client);
	}
	else {
		std::string message = "Error: Invalid channel name.\r\n";
		servSendMessageToClient(message, client);
	}
}