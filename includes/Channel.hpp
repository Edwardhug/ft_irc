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
    std::vector<Client*> _clientsInvited;
    std::map<char, bool> _modes;
    std::string _password;
    unsigned int _maxClient;

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
    void setTopic(std::string topic);
    std::string getTopic();
    bool checkPerm(char mode);
    bool checkOperator(Client &client);
    void addModes(char mode, Client& from, std::string target);
    void delModes(char mode, Client& from, std::string target);
    bool clientInChannel(Client &toFind);
    void removeClient(Client &toRemove);
    void addClientInvited(Client* newClient);
    bool clientIsInvited(Client& client);
    std::string getPass();
    bool checkModesForJoin(Client& client);

    void displayFDS();
};

#endif