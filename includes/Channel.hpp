#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include "Client.hpp"
#include <iostream>
#include "../includes/Server.hpp"
#include "lib.hpp"
class Client;
class Server;
class Channel {
    private:
    std::string _name;
    std::string _topic;
    std::vector<Client*> _clients;
    std::vector<Client*> _operators;
    std::map<char, bool> _modes;
    std::string _password;
    int _maxClient;

    public:
	Channel();
    Channel(std::string name, Client* creator);
    ~Channel();
    void changeMode(char addOrDel, char mode, Client& from, std::string target);
	void	addClient(Client *newClient);
    std::string getName() const;
	std::vector<Client*> getVecClient();
    void deleteOperator(std::string target);
    void addOperator(std::string target);
    void addModes(char mode, Client& from, std::string target);
    void delModes(char mode, Client& from, std::string target);
    bool clientInChannel(Client &toFind);
    bool checkOperator(Client &client);
};

#endif