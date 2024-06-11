#include "../includes/lib.hpp"
#include "../includes/Error.hpp"

//===================PRIVMSG========================
void    Server::sendmsg(const std::string &from, const std::string &to, const std::string& message) // il n'affiche pas qui a envoyer le message
{
    Client* clientTo;
    Client* clientFrom;
    try {
        clientFrom = &findClientWithNick(from);
    }
    catch (std::runtime_error& e){
        std::cout << e.what() << std::endl;
    }
    try {
        clientTo = &findClientWithNick(to);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return ERR_NOSUCHNICK(*clientFrom, const_cast<std::string&>(to));
    }
    std::string completeMessage = ":" + from + " PRIVMSG " + to + " :" + message + "\r\n";
    servSendMessageToClient(completeMessage, *clientFrom);
    (void)clientTo;
}

void    Server::splitForPrivMsg(const std::string &buff, int fdSender)
{
    std::deque<std::string> data;
    data = split(buff, ' ');
    Client *from;
    try {
        from = &findClientWithFd(fdSender);
    }
    catch (std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return ;
    }
    if (data.size() < 3)
        return ERR_NEEDMOREPARAMS(*from, "PRIVMSG");
    {
        std::string to = data[1];
        if (!nickAlreadyExist(to))
            return ERR_NOSUCHNICK(*from, to);
        std::string message = data[2];
        if (message.empty())
            return ERR_NOTEXTTOSEND(*from);
        if (!message.empty() && message[0] == ':')
        {
            message.erase(0, 1);
        }
        sendmsg(from->getNick(), to, message);
    }
}

