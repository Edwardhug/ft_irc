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

void Server::addChannel(const Channel &channel) {
	_vecChannel.push_back(channel);
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

void Server::servSendMessageToClient(const std::string &message, Client &client)
{
    send(client.getFdClient(), message.c_str(), message.length(), 0);
}

void Server::errorPassword(Client& client)
{
    servSendMessageToClient(":server 464 " + client.getNick() + " :Password incorrect\r\n", client);
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

Client&	Server::findClientWithNick(const std::string &nick)
{
	size_t i;
    for (i = 0; i < _vecClient.size(); i++)
    {
        if (_vecClient[i].getNick() == nick)
        {
            return _vecClient[i];
        }
    }
	throw std::runtime_error("Client not found with the nick");
}

Client& Server::findClientWithFd(int fd)
{
	size_t i;
    for (i = 0; i < _vecClient.size(); i++)
    {
        if (_vecClient[i].getFdClient() == fd)
        {
            return _vecClient[i];
        }
    }
	throw std::runtime_error("Client not found with the fd");
}

void	Server::readReceivedData(int fd)
{
    static std::string completeBuffer;
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
        completeBuffer += buffer;
        if (completeBuffer.find('\n') != std::string::npos)
        {
            std::cout << "begin : \n" << completeBuffer << "\n end\n";
            std::vector<std::string> splittedBuffer = splitBuffer(completeBuffer, '\n');
            completeBuffer.erase();
            for (size_t i = 0; i < splittedBuffer.size(); i++)
            {
                if (splittedBuffer[i].find('\r') != std::string::npos)
                    splittedBuffer[i].erase(splittedBuffer[i].find('\r'), 1);
                //splittedBuffer[i] += "\r\n";
                operatorCanals(splittedBuffer[i].c_str(), fd);
            }
            splittedBuffer.erase(splittedBuffer.begin(), splittedBuffer.end());
        }
	}
}

//===================NICK===========================
bool    Server::attributeNickName(int fd, const char *buffer)
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
                std::string newNick = find.substr(nickPos, endLinePos - nickPos);
                if (newNick.find(13) != std::string::npos)
                    newNick = newNick.substr(0, newNick.size() - 1);
                _vecClient[i].setNick(newNick);
                std::cout << "The nick is : " << _vecClient[i].getNick() << std::endl;
                break;
            }
        }
        return true;
    }
    return false;
}
//==================================================

//===================PASS===========================
void    Server::checkPass(const std::string &buff, int fdClient)
{
    std::string rightPass = this->_password;
    size_t passWord = buff.find("PASS ");
    if (passWord == std::string::npos)
    {
        std::cout << "Error with command PASS, no \'PASS\'\n";
        return;
    }
    size_t passEnd = buff.find('\n', passWord);
    if (passEnd == std::string::npos)
    {
        std::cout << "Error with command PASS, no \'\\n\'\n";
        return;
    }
    std::string passIn = buff.substr(passWord + 5, passEnd - passWord - 5);
    if (!passIn.empty() && passIn[passIn.size() - 1] == '\r')
    {
        passIn.erase(passIn.size() - 1);
    }
    if (passIn != rightPass)
    {
        errorPassword(findClientWithFd(fdClient));
//        close(fdClient);
//        clearClient(fdClient);
    }
    else {
        std::cout << "PASS CORRECT\n";
        for (size_t i = 0; i < _vecClient.size(); i++)
        {
            if (fdClient == _vecClient[i].getFdClient())
            {
                _vecClient[i].setPass();
            }
        }
    }
}
//==================================================

//===================PRIVMSG========================
void    Server::sendmsg(const std::string &from, const std::string &to, const std::string& message) // il n'affiche pas qui a envoyer le message
{
    int fd;
    try {
        fd = findClientWithNick(to).getFdClient();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        throw std::exception(); // A changer
    }
    std::string completeMessage = ":" + from + " PRIVMSG " + to + " :" + message + "\r\n";
    ssize_t bytesSend;
    bytesSend = send(fd, completeMessage.c_str(), completeMessage.length(), 0);// peut etre changer les flags
    if (bytesSend == -1)
    {
        std::cout << "Error when send data for a PRIVMSG" << std::endl;
        throw std::exception(); // afficher erreur envoie des donnees
    }
    // std::cout << "bytesSend : " << bytesSend << std::endl;
}

void    Server::splitForPrivMsg(const std::string &buff, int fdSender)
{
    std::vector<std::string> data;
    data = split(buff, ' ');
    if (data.size() >= 3)
    {
        std::string to = data[1];
        std::string message = data[2];
        Client from;
        try {
            from = findClientWithFd(fdSender);
        }
        catch (std::runtime_error& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::exception(); //A changer
        }
        std::cout << from.getPass() << std::endl;
        if (!from.getPass())
        {
            errorPassword(from);
            return;
        }
        if (!message.empty() && message[0] == ':')
        {
            message.erase(0, 1);
        }
        sendmsg(from.getNick(), to, message);
    }
}
//====================================================
//=======================MODE=========================
void    Server::splitForMode(const std::string &buff, int fdSender)
{
    std::string target;
    std::string data = buff.substr(buff.find("MODE") + 5);
    std::vector<std::string> datas = split(data, ' ');
    if (datas.size() == 1)
    {
        //peut etre afficher un message
        return ;
    }
    if (datas.size() == 3)
    {
        target = datas[2];
    }
    std::string channel = datas[0];
    std::string what = datas[1];
    Client from;
    try {
        from = findClientWithFd(fdSender);
    }
    catch (std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }
    if (!from.getPass())
        errorPassword(from);
    std::cout << "channel :" <<  channel << "modes : " << what << std::endl;
    if (what.find('-') == std::string::npos && what.find('+') == std::string::npos)
    {
        std::cerr << "Add - or + before the channel operator";
        return ;
    }
    for (size_t i = 0; i < this->_vecChannel.size(); i++)
    {
        if (channel == _vecChannel[i].getName())
        {
            char addOrDel = what[0];
            char mode = what[1];
            if (target.empty())
                _vecChannel[i].changeMode(addOrDel, mode, from, NULL);
            else
                _vecChannel[i].changeMode(addOrDel, mode, from, target);
        }
    }
}

//====================================================
void    Server::operatorCanals(const char *buffer, int fdSender) // A transformer en switch case ?
{
    std::string buff = static_cast<std::string>(buffer);

    if (buff.find("PRIVMSG") != std::string::npos && buff.size() > 8 && buff[8] == '#')
    {
        channelMsg(const_cast<char*>(buffer), fdSender);
    }
    else if (buff.find("NICK") != std::string::npos)
    {
        attributeNickName(fdSender, buffer);
    }
    else if (buff.find("PRIVMSG") != std::string::npos)
    {
        splitForPrivMsg(buff, fdSender);
    }
    else if (buff.find("MODE") != std::string::npos)
    {
        splitForMode(buff, fdSender);
    }
	else if (buff.find("JOIN") != std::string::npos){
		splitForJoin(buff, fdSender);
	}
    else if (buff.find("PASS") != std::string::npos)
    {
        checkPass(buff, fdSender);
    }
    else if (buff.find("TOPIC") != std::string::npos) {
        splitForTopic(buff, fdSender);
    }
    else if (buff.find("INVITE") != std::string::npos)
    {
        inviteClient(buff, fdSender);
    }
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
