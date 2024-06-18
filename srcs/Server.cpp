#include "Server.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "../includes/lib.hpp"
#include "../includes/ErrorAndReply.hpp"
#include <cstdio>

Server::Server(int port, std::string password) : _port(port), _password(password)
{
    _serverSocketFd = -1;
}

Server::~Server()
{}

void Server::setPort(int port)
{
    this->_port = port;
}

void Server::setServerSocketFd(int fd)
{
    this->_serverSocketFd = fd;
}

void Server::addClient(const Client &client)
{
    _vecClient.push_back(client);
}

void Server::addPollFd(const pollfd &fd)
{
    _vecPollFd.push_back(fd);
}

void Server::addChannel(const Channel &channel) {
	_vecChannel.push_back(channel);
}

void Server::deleteChannel(const std::string &ch)
{
    for (size_t i = 0; i < _vecChannel.size(); i++)
    {
        if (_vecChannel[i].getName() == ch)
            _vecChannel.erase(_vecChannel.begin() + i);
    }
}

void Server::clearClient(int fd)
{
    for (size_t i = 0; i < _vecChannel.size(); i++)
    {
        if (_vecChannel[i].clientInChannelFd(fd)) {
            Client* c;
            try {
                c = &findClientWithFd(fd);
            }
            catch (std::runtime_error& e)
            {
                std::cout << e.what() << std::endl;
            }
            _vecChannel[i].removeClientFd(fd);
            c->setNoChannelActive();
            if (!_vecChannel[i].sendNamesInChannel(*c))
            {
                _vecChannel[i].closeChannel();
                deleteChannel(_vecChannel[i].getName());
            }
        }
    }
    for (size_t i = 0; i < _vecClient.size(); i++)
    {
        if (_vecClient[i].getFdClient() == fd)
        {
            _vecClient.erase(_vecClient.begin() + i);
            break ;
        }
    }
    for (size_t i = 0; i < _vecPollFd.size(); i++)
    {
        if (_vecPollFd[i].fd == fd)
        {
            _vecPollFd.erase(_vecPollFd.begin() + i);
            break ;
        }
    }
}


void Server::closeFds()
{
    std::deque<Client>::iterator it;

    for (it = _vecClient.begin(); it != _vecClient.end(); it++)
    {
        close(it->getFdClient());
    }
    if (_serverSocketFd != -1)
    {
        close(_serverSocketFd);
    }
}



void Server::servInit() {
    sockaddr_in data_sock;
    pollfd newPoll;

    data_sock.sin_family = AF_INET;
    data_sock.sin_port = htons(this->_port);
    data_sock.sin_addr.s_addr = INADDR_ANY;

    this->_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_serverSocketFd == -1)
    {
        std::cout << "Error: socket function as failed for server socket" << std::endl;
        throw std::exception();
    }
    int biz = 1;
    if (setsockopt(this->_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &biz, sizeof(biz)) == -1)
    {
        std::cout << "Error: setsockopt as failed for server socket" << std::endl;
        close(_serverSocketFd);
        throw std::exception();
    }
    if (fcntl(_serverSocketFd, F_SETFL, SOCK_NONBLOCK) == -1)
    {
        std::cout << "Error: fcntl as failed for server socket" << std::endl;
        close(_serverSocketFd);
        throw std::exception();
    }
    if (bind(_serverSocketFd, (struct sockaddr *)&data_sock, sizeof(data_sock)) == -1)
    {
        std::cout << "Error: bind as failed for server socket" << std::endl;
        close(_serverSocketFd);
        throw std::exception();
    }
    if (listen(_serverSocketFd, SOMAXCONN) == -1)
    {
        std::cout << "Error: listen as failed to listen on server socket" << std::endl;
        close(_serverSocketFd);
        throw std::exception();
    }

    newPoll.fd = _serverSocketFd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;

    this->addPollFd(newPoll);
}

void	Server::addNewClient() {
	Client		newClient;
	sockaddr_in	infoClient;
	socklen_t 	lenStructClient = sizeof(infoClient);
	pollfd		pollClient;

	int fdClient = accept(_serverSocketFd, (sockaddr*)&infoClient, &lenStructClient);
	if (fdClient < 0) {
		std::cout << "The server didn't accepted the connection" << std::endl;
		return ;
	}
	if (fcntl(fdClient, F_SETFL, SOCK_NONBLOCK) == -1)
    {
        close(fdClient);
        std::cout << "Error during fcntl call" << std::endl; return ;
    }
	pollClient.fd = fdClient;
	pollClient.events = POLLIN;
	pollClient.revents = 0;

	newClient.setFdClient(fdClient);
	newClient.setIpAddr(inet_ntoa(infoClient.sin_addr));
	_vecClient.push_back(newClient);
	_vecPollFd.push_back(pollClient);
	std::cout << "new client added" << std::endl;
}

void	Server::readReceivedData(int fd)
{
    static std::string completeBuffer;
	char buffer[BUFFER_SIZE];
	ssize_t bytes_received;
	bytes_received = recv(fd, buffer, BUFFER_SIZE, 0);
	if (bytes_received == -1) {
		std::cout << "error during recv call" << std::endl;
		return ;
	}
	else if (bytes_received == 0) {
		std::cout << "Client " << fd << " disconnected" << std::endl;
		clearClient(fd);
		close(fd);
	}
	else {
		buffer[bytes_received] = '\0';
        completeBuffer += buffer;
        if (completeBuffer.find('\n') != std::string::npos)
        {
            std::deque<std::string> splittedBuffer = splitBuffer(completeBuffer, '\n');
            completeBuffer.erase();
            for (size_t i = 0; i < splittedBuffer.size(); i++)
            {
                if (splittedBuffer[i].find('\r') != std::string::npos)
                    splittedBuffer[i].erase(splittedBuffer[i].find('\r'), 1);
                operatorCanals(splittedBuffer[i].c_str(), fd);
            }
            splittedBuffer.erase(splittedBuffer.begin(), splittedBuffer.end());
        }
	}
}

void    Server::operatorCanals(const char *buffer, int fdSender) {
    std::string buff = static_cast<std::string>(buffer);

    if (buff.find("PASS") != std::string::npos) {
        checkPass(buff, fdSender);
        return ;
    }
    Client *from;
    try {
        from = &findClientWithFd(fdSender);
    }
    catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        return;
    }
    if (from->getPass()) {
        if (buff.find("VLAD") != std::string::npos && buff.find("PRIVMSG") != std::string::npos && buff.size() > 8 && buff[8] == '#') {
            sendWeatherRequest(buff, fdSender);
        } else if (buff.find("PRIVMSG") != std::string::npos && buff.size() > 8 && buff[8] == '#') {
            channelMsg(const_cast<char *>(buffer), fdSender);
        } else if (buff.find("PRIVMSG") != std::string::npos) {
            splitForPrivMsg(buff, fdSender);
        } else if (buff.find("MODE") != std::string::npos) {
            splitForMode(buff, fdSender);
        } else if (buff.find("JOIN") != std::string::npos) {
            splitForJoin(buff, fdSender);
        } else if (buff.find("TOPIC") != std::string::npos) {
            splitForTopic(buff, fdSender);
        } else if (buff.find("INVITE") != std::string::npos) {
            inviteClient(buff, fdSender);
        } else if (buff.find("KICK") != std::string::npos) {
            kickClient(buff, fdSender);
        } else if (buff.find("USER") != std::string::npos) {
            setUsername(fdSender, buff);
        } else if (buff.find("NICK") != std::string::npos) {
            attributeNickName(fdSender, buff);
        }
    }
}

void	Server::servLoop()
{
	while (g_signal == false)
	{
		if (poll(&_vecPollFd[0], _vecPollFd.size(), -1) == -1 && g_signal == false) {
			std::cout << "poll error" << std::endl;
			throw std::exception();
		}
		for (size_t i = 0; i < _vecPollFd.size(); i++) {
			if (_vecPollFd[i].revents & POLLIN) {
				if (_vecPollFd[i].fd == _serverSocketFd)
					addNewClient();
				else
					readReceivedData(_vecPollFd[i].fd);
			}
		}
	}
}
