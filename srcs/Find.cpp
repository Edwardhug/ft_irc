#include "../includes/lib.hpp"
#include "../includes/ErrorAndReply.hpp"

Client&	Server::findClientWithNick(const std::string &nick)
{
	size_t i;
    for (i = 0; i < _vecClient.size(); i++)
    {
        if (_vecClient[i].getNick() == nick)
        {
            return _vecClient[i];
        }
    }
	throw std::runtime_error("Client not found with the nick");
}

Client& Server::findClientWithFd(int fd)
{
	size_t i;
    for (i = 0; i < _vecClient.size(); i++)
    {
        if (_vecClient[i].getFdClient() == fd)
        {
            return _vecClient[i];
        }
    }
	throw std::runtime_error("Client not found with the fd");
}

Channel &Server::findChannelWithName(std::string name) {
	size_t i;
    for (i = 0; i < _vecChannel.size(); i++)
    {
        if (_vecChannel[i].getName() == name) {
			return _vecChannel[i];
		}
    }
    throw std::runtime_error("Channel not found with the name");
}