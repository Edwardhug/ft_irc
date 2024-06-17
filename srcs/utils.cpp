#include "lib.hpp"

std::deque<std::string> splitAllSpaces(std::string &str) {
	std::deque<std::string> data;
	std::string word;
	std::istringstream iss(str);
	while (iss >> word) {
		data.push_back(word);
	}
	return data;
}

std::deque<std::string> splitBuffer(const std::string& str, char delimiter) {
    std::deque<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::deque<std::string> split(const std::string& str, char delimiter) {
    std::deque<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
        if (tokens.size() == 3) {
            std::string remaining;
            std::getline(tokenStream, remaining);
            tokens[2] += delimiter + remaining;
            break;
        }
    }
    return tokens;
}

int ft_atoi(const char* str) {
    if (!str) {
        return 0;
    }
    while (std::isspace(*str)) {
        ++str;
    }
    int sign = 1;
    if (*str == '-' || *str == '+') {
        if (*str == '-') {
            sign = -1;
        }
        ++str;
    }
    int result = 0;
    while (std::isdigit(*str)) {
        result = result * 10 + (*str - '0');
        ++str;
    }

    return sign * result;
}

bool strIsDigit(std::string &str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (isdigit(str[i]) == 0)
            return false;
    }
    return true;
}

bool servSendMessageToClient(const std::string &message, Client &client)
{
    ssize_t bytesSent = send(client.getFdClient(), message.c_str(), message.length(), 0);
    if (bytesSent == -1)
    {
        std::cerr << RED << "Error when sending data to client " << client.getNick() << ": ";
        perror("");
        std::cerr << RESET;
        return false;
    }
    return true;
}

char *ft_ftoa(double f) {
	char *buf = new char[100];
	sprintf(buf, "%f", f);
	return buf;
}
