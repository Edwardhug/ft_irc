#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
class Client
{
    private:
    int _fdClient;
    std::string _ipAddrClient;

    public:
    Client();
    ~Client();
    // Client(const Client &c);
    // Client &operator=(const Client &c);
    void setFdClient(int fd);
    void setIpAddr(const std::string &ipAddr);
    int getFdClient();
    std::string getIpAddr();
};

#endif