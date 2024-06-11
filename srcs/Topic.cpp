#include "../includes/Server.hpp"
#include "../includes/lib.hpp"
#include "../includes/ErrorAndReply.hpp"


void	Server::splitForTopic(std::string buff, int fdSender) {
	std::deque<std::string> data;
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
	if (channelExist(data[1]) == false) {
		return ERR_NOSUCHCHANNEL(*client, data[1]);
	}
	Channel &channel = findChannelWithName(data[1]);
	std::string channelName = channel.getName();
	if (data.size() >= 3 && data[2] == ":: " && channelExist(data[1]) == true) {
		if (channel.checkPerm('t') == true && channel.checkOperator(*client) == false ) {
			return ERR_CHANOPRIVSNEEDED(*client, channel.getName()); //? pas sur que ce soit le bon code d'erreur
		}
		channel.setTopic("");
		std::string message = ":Server NOTICE " + channelName + " Channel topic is now clear." + "\r\n";
		if (!servSendMessageToClient(message, *client))
            return ;
	}
	else if (data.size() >= 3 && channelExist(data[1]) == true) {
		if (channel.checkPerm('t') == true && channel.checkOperator(*client) == false ) {
			return ERR_CHANOPRIVSNEEDED(*client, channel.getName()); //? pas sur que ce soit le bon code d'erreur
		}
		channel.setTopic(data[2]);
		std::string message = ":Server NOTICE " + channelName + " Setting the new topic on \""+ channelName + "\" to \"" + channel.getTopic() + "\".\r\n";
		if (!servSendMessageToClient(message, *client))
            return ;
	}
	else if (data.size() == 2 && channelExist(data[1]) == true) {
		std::string message = ":Server NOTICE " + channelName + " Channel topic is \"" + channel.getTopic() + "\".\r\n";
		if (!servSendMessageToClient(message, *client))
            return ;
	}
	else {
		return ERR_NOSUCHCHANNEL(*client, data[1]);
	}
}

// void Server::splitForTopic(std::string buff, int fdSender) {
// 	std::vector<std::string> data;
// 	Client &client = findClientWithFd(fdSender);
// 	data = split(buff, ' ');
// 	if (data.size() >= 3 && channelExist(data[1]) == true) {
// 		Channel &channel = findChannelWithName(data[1]);
// 		if (channel.checkPerm('t') == true && channel.checkOperator(client) == false ) {
// 			std::string message = "Error: Permission denied.\r\n";
// 			servSendMessageToClient(message, client);
// 			return;
// 		}
// 		channel.setTopic(data[2]);
// 		std::string channelName = channel.getName();
// 		std::string message = ":" + client.getNick() + " TOPIC " + channelName + " " + channel.getTopic() + "\r\n";
// 		servSendMessageToClient(message, client);
// 	}
// 	else if (data.size() == 2 && channelExist(data[1]) == true) {
// 		Channel &channel = findChannelWithName(data[1]);
// 		std::string channelName = channel.getName();
// 		std::string message = ":" + client.getNick() + " TOPIC " + channelName + " " + channel.getTopic() + "\r\n";
// 		servSendMessageToClient(message, client);
// 	}
// 	else {
// 		std::string message = "Error: Invalid channel name.\r\n";
// 		servSendMessageToClient(message, client);
// 	}
// }
 