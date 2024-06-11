#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <deque>
#include <map>
#include "Client.hpp"
#include <iostream>
#include "../includes/Server.hpp"
#include "lib.hpp"
#include "Bot.hpp"

class Client;
class Server;
class Bot;
class Channel {
    private:
    std::string _name;
    std::string _topic;
    std::deque<Client*> _clients;
    std::deque<Client*> _operators;
    std::deque<Client*> _clientsInvited;
    std::map<char, bool> _modes;
    std::string _password;
    unsigned int _maxClient;
	Bot *_bot;

    public:
	Channel();
    Channel(std::string name, Client* creator, Bot *bot);
    ~Channel();
    void changeMode(char addOrDel, char mode, Client& from, std::string target);
	void	addClient(Client *newClient);
    std::string getName() const;
	std::deque<Client*> getVecClient();
    void deleteOperator(std::string target, Client& from);
    void addOperator(std::string target, Client& from);
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
    bool checkOperatorWithName(std::string nick);
    void displayFDS();
    void msgToChannel(std::string msg);
    std::map<char, bool> getModes();
    unsigned int getMaxClient();
};

#endif