#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "Channel.hpp"

class Channel;

class Client
{
    private:
    int _fdClient;
    std::string _ipAddrClient;
    std::string _nick;
	bool		_inChannel;
	Channel		*_activeChannel;

    public:
    Client();
    ~Client();
    // Client(const Client &c);
    // Client &operator=(const Client &c);
    void setFdClient(int fd);
    void setIpAddr(const std::string &ipAddr);
    int getFdClient();
    std::string getIpAddr();
    void setNick(std::string nick);
    std::string getNick();
	bool	getInChannel();
	void	changeChannelBool();
	void	setChannel(Channel &channel);
	Channel getActiveChannel();
};

#endif