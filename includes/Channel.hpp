#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include "Client.hpp"

class Client;

class Channel {
    private:
    std::string _name;
    std::string _topic;
    std::vector<Client*> _clients;
    std::vector<Client*> _operators;
    std::map<char, bool> _modes;

    public:
	Channel();
    Channel(std::string name, Client* creator);
    ~Channel();
    void changeMode(char addOrDel, char mode, Client& from);
	void	addClient(Client *newClient);
    std::string getName() const;
	std::vector<Client*> getVecClient();
};

#endif