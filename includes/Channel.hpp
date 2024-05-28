#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "Client.hpp"
class Channel {
    private:
    std::string _name;
    std::string _topic;
    std::vector<Client*> _clients;
    std::vector<Client*> _operators;

    public:
    Channel(std::string name, Client* creator);
    ~Channel();
    void changeMode(char addOrDel, std::string modes);
	void	addClient(Client *newClient);
};

#endif