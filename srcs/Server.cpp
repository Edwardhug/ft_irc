#include "Server.hpp"
#include <unistd.h>
#include <fcntl.h>
#include "../includes/lib.hpp"

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

void Server::servInit() {
    sockaddr_in data_sock;
    pollfd      newPoll;

    data_sock.sin_family = AF_INET; // IPV4
    data_sock.sin_port = htons(this->_port);
    data_sock.sin_addr.s_addr = INADDR_ANY; // Prends toutes les addresses locales (test un fix)

    this->_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_serverSocketFd == -1)
        throw std::exception();
    int biz = 1;
    if (setsockopt(this->_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &biz, sizeof(biz)) == -1) // On definit le socket pour pouvoir reutilliser l'addresse
    {
        close(_serverSocketFd);
        throw std::exception();
    }
    if (fcntl(_serverSocketFd, F_SETFL, SOCK_NONBLOCK) == -1) // Le socket ne block plus lors d'une tache 
    {
        close(_serverSocketFd);
        throw std::exception();
    }
    if (bind(_serverSocketFd, (struct sockaddr *)&data_sock, sizeof(data_sock)) == -1) //  On bind le socket avec le port et l'addresse 
    {
        close(_serverSocketFd);
        throw std::exception();
    }
    if (listen(_serverSocketFd, SOMAXCONN) == -1) // Le socket accepte les nouvelles connections et max 1024
    {
        close(_serverSocketFd);
        throw std::exception();
    }

    newPoll.fd = _serverSocketFd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;

    this->addPollFd(newPoll);
}

void	Server::servLoop()
{
	while (g_signal == false)
	{
		if (poll(&_vecPollFd[0], _vecPollFd.size(), -1) == -1) { //va bloquer jusqu'a recevoir une donnee
			std::cout << "poll error" << std::endl;
			throw std::exception();
		}
		for (int i = 0; i < _vecPollFd.size(); i++) {
			if (_vecPollFd[i].revents & POLLIN) { // on verifie si le bit de POLLIN est dans revent, comme ca on check si il y a bien des trucs a lire
				if (_vecPollFd[i].fd = _serverSocketFd) //regarde si on a le meme file descriptor que celui du socket du serveur
					// le client est nouveau
				else
					// le client existe deja
			}
		}
	}
}