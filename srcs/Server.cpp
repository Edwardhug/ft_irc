#include "Server.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
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

void Server::clearClient(int fd) //? Peut etre closes les fds
{
    for (size_t i = 0; i < _vecClient.size(); i++)
    {
        if (_vecClient[i].getFdClient() == fd)
        {
            std::cout << _vecClient[i].getIpAddr() << std::endl;
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
    std::vector<Client>::iterator it;

    for (it = _vecClient.begin(); it != _vecClient.end(); it++)
    {
        //? Ecrire un message 
        close(it->getFdClient());
    }
    if (_serverSocketFd != -1)
    {
        //? Ecrire un message
        close(_serverSocketFd);
    }
}

void Server::servInit() {
    sockaddr_in data_sock;
    pollfd newPoll;

    data_sock.sin_family = AF_INET; // IPV4
    data_sock.sin_port = htons(this->_port);
    data_sock.sin_addr.s_addr = INADDR_ANY; // Prends toutes les addresses locales (test un fix)

    this->_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_serverSocketFd == -1)
    {
        std::cout << "Error: socket function as failed for server socket" << std::endl;
        throw std::exception();
    }
    int biz = 1;
    if (setsockopt(this->_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &biz, sizeof(biz)) == -1) // On definit le socket pour pouvoir reutilliser l'addresse
    {
        std::cout << "Error: setsockopt as failed for server socket" << std::endl;
        close(_serverSocketFd);
        throw std::exception();
    }
    if (fcntl(_serverSocketFd, F_SETFL, SOCK_NONBLOCK) == -1) // Le socket ne block plus lors d'une tache 
    {
        std::cout << "Error: fcntl as failed for server socket" << std::endl;
        close(_serverSocketFd);
        throw std::exception();
    }
    if (bind(_serverSocketFd, (struct sockaddr *)&data_sock, sizeof(data_sock)) == -1) //  On bind le socket avec le port et l'addresse 
    {
        std::cout << "Error: bind as failed for server socket" << std::endl;
        close(_serverSocketFd);
        throw std::exception();
    }
    if (listen(_serverSocketFd, SOMAXCONN) == -1) // Le socket accepte les nouvelles connections et max 1024
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
	socklen_t 	lenStructClient = sizeof(infoClient);	// recupere la taille de la struct qui contient les infos de client
	pollfd		pollClient;

	int fdClient = accept(_serverSocketFd, (sockaddr*)&infoClient, &lenStructClient); //tente de connecter le client au serveur
	if (fdClient < 0) {
		std::cout << "The server didn't accepted the connection" << std::endl;
		return ;
	}
	if (fcntl(fdClient, F_SETFL, SOCK_NONBLOCK) == -1)	// met le socket du client en mode non bloquant
    {
        close(fdClient);
        std::cout << "Error during fcntl call" << std::endl; return ;
    }
	pollClient.fd = fdClient;	// remplit la struct poll avant de l'ajouter au vecteur
	pollClient.events = POLLIN;
	pollClient.revents = 0;

	newClient.setFdClient(fdClient);
	newClient.setIpAddr(inet_ntoa(infoClient.sin_addr)); // converti l'adress en binaire au format std::string
	_vecClient.push_back(newClient);
	_vecPollFd.push_back(pollClient);
	std::cout << "new client added" << std::endl;
}

bool    Server::attributeNickName(int fd, char *buffer)
{
    std::string find;
    size_t nickPos;
    size_t endLinePos;
    find = static_cast<std::string>(buffer);
    if (find.find("NICK") != std::string::npos)
    {
        for (size_t i = 0; i < _vecClient.size(); i++)
        {
            if (_vecClient[i].getFdClient() == fd)
            {
                nickPos = find.find("NICK");
                nickPos += 5;
                endLinePos = find.find('\n', nickPos);
                if (endLinePos == std::string::npos)
                    endLinePos = find.length();
                _vecClient[i].setNick(find.substr(nickPos, endLinePos - nickPos - 1));
                std::cout << "The nick is : " << _vecClient[i].getNick() << std::endl;
                break;
            }
        }
        return true;
    }
    return false;
}

Client	Server::findClientWithNick(const std::string &nick)
{
	size_t i;
    for (i = 0; i < _vecClient.size(); i++)
    {
        if (_vecClient[i].getNick() == nick)
        {
            return _vecClient[i];
        }
    }
	return _vecClient[i];
}

Client Server::findClientWithFd(int fd)
{
	size_t i;
    for (i = 0; i < _vecClient.size(); i++)
    {
        if (_vecClient[i].getFdClient() == fd)
        {
            return _vecClient[i];
        }
    }
	return _vecClient[i];
}

// Client	Server::

void	Server::readReceivedData(int fd)
{
	char buffer[BUFFER_SIZE];
	ssize_t bytes_received;
 	// for (int i = 0; i < BUFFER_SIZE; i++) {
	// 	buffer[i] = 0;
	// }
	bytes_received = recv(fd, buffer, BUFFER_SIZE, 0); //  put the received data in the buffer
	if (bytes_received == -1) {
		std::cout << "error during recv call" << std::endl;
		return ;
	}
	else if (bytes_received == 0) {
		std::cout << "Client " << fd << " disconnected" << std::endl;
		// faut peut etre faire quelque chose mais je sais pas quoi
		clearClient(fd);
		close(fd);
	}
	else {
		buffer[bytes_received] = '\0';
		std::cout << "Client " << fd << " said : " << buffer << std::endl;
        if (attributeNickName(fd, buffer))
            return;
        operatorCanals(buffer, fd);
	}
}
//===================PRIVMSG========================
void    Server::sendmsg(const std::string &from, const std::string &to, const std::string& message) // il n'affiche pas qui a envoyer le message
{
    int fd = findClientWithNick(to).getFdClient();
    if (fd == -1)
    {
        std::cout << "Not find the fd of the receiver" << std::endl;
        throw std::exception(); // afficher fd non trouver
    }
    std::string completeMessage = ":" + from + " PRIVMSG " + to + " :" + message + "\r\n";
    ssize_t bytesSend;
    bytesSend = send(fd, completeMessage.c_str(), completeMessage.length(), 0);// peut etre changer les flags
    if (bytesSend == -1)
    {
        std::cout << "Error when send data for a PRIVMSG" << std::endl;
        throw std::exception(); // afficher erreur envoie des donnees
    }
    std::cout << "bytesSend : " << bytesSend << std::endl;
}

void    Server::splitForPrivMsg(std::string buff, int fdSender)
{
    std::vector<std::string> data;
    data = split(buff, ' ');
    if (data.size() >= 3)
    {
        std::string to = data[1];
        std::string message = data[2];
        std::string from = findClientWithFd(fdSender).getNick();
        if (from.empty())
        {
            std::cout << "Error: sender nickname not found";
            throw std::exception(); // pas sur qu'elle existe mais rajouter envoyer non trouver
        }
        if (!message.empty() && message[0] == ':')
        {
            message.erase(0, 1);
        }
        sendmsg(from, to, message);
    }
}
//====================================================
//=======================MODE=========================
void    Server::splitForMode(std::string buff, int fdSender)
{
    std::vector<std::string> data;
    data = split(buff, ' ');
    (void)fdSender;
    if (data.size() >= 3)
    {
        std::string channel = data[1];
        std::string modes = data[2];
        std::string from = findClientWithFd(fdSender).getNick();
        std::cout << "channel :" <<  channel << "modes : " << modes << std::endl;
        if (modes.find('-') == std::string::npos && modes.find('+') == std::string::npos)
        {
            std::cerr << "Add - or + before the channel operator";
        }
        std::string message = ":" + from + " MODE " + channel + " " + modes + "\r\n";
    }
}

//====================================================
void    Server::operatorCanals(char *buffer, int fdSender) // A transformer en switch case ?
{
    std::string buff = static_cast<std::string>(buffer);

    if (buff.find("PRIVMSG") != std::string::npos)
    {
        splitForPrivMsg(buff, fdSender);
    }
    if (buff.find("MODE") != std::string::npos)
    {
        splitForMode(buff, fdSender);
    }
	// else if (buff.find("JOIN"))
	// {
	// 	data = split(buff, ' ');
    //     if (data.size() >= 2)
    //     {
	// 		Channel	newChannel(data[2], )
	// 	}
	// }
}

void	Server::servLoop()
{
	while (g_signal == false)
	{
		if (poll(&_vecPollFd[0], _vecPollFd.size(), -1) == -1 && g_signal == false) { //va bloquer jusqu'a recevoir une donnee
			std::cout << "poll error" << std::endl;
			throw std::exception();
		}
		for (size_t i = 0; i < _vecPollFd.size(); i++) {
			if (_vecPollFd[i].revents & POLLIN) { // on verifie si le bit de POLLIN est dans revent, comme ca on check si il y a bien des trucs a lire
				if (_vecPollFd[i].fd == _serverSocketFd) //regarde si on a le meme file descriptor que celui du socket du serveur
					addNewClient();
				else
					readReceivedData(_vecPollFd[i].fd);
			}
		}
	}
}
