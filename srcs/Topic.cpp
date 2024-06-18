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
			return ERR_CHANOPRIVSNEEDED(*client, channel.getName());
		}
		channel.setTopic("");
		std::string message = ":Server NOTICE " + channelName + " Channel topic is now clear." + "\r\n";
		if (!servSendMessageToClient(message, *client))
            return ;
	}
	else if (data.size() >= 3 && channelExist(data[1]) == true) {
		if (channel.checkPerm('t') == true && channel.checkOperator(*client) == false ) {
			return ERR_CHANOPRIVSNEEDED(*client, channel.getName());
		}
		channel.setTopic(data[2]);
		std::string rpl = ":server 332 " + client->getNick() + " " + channelName + " :" + channel.getTopic() + "\r\n";
		sendMessageToChannel(channelName, rpl);
	}
	else if (data.size() == 2 && channelExist(data[1]) == true) {
		std::string rpl = ":server 332 " + client->getNick() + " " + channelName + " :" + channel.getTopic() + "\r\n";
		sendMessageToChannel(channelName, rpl);
	}
	else {
		return ERR_NOSUCHCHANNEL(*client, data[1]);
	}
}
 