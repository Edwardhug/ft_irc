#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
class Client
{
    private:
    int _fdClient;
    std::string _ipAddrClient;
    std::string _nick;
    bool _corrPass;

    public:
    Client();
    ~Client();
    // Client(const Client &c);
    // Client &operator=(const Client &c);
    void setFdClient(int fd);
    void setIpAddr(const std::string &ipAddr);
    int getFdClient() const;
    std::string getIpAddr() const;
    void setNick(std::string nick);
    std::string getNick() const;
    bool getPass() const;
    void setPass();
};

#endif