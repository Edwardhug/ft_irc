#include "../includes/Bot.hpp"
#include "../includes/Server.hpp"
#include "../includes/lib.hpp"
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <memory>

Bot::Bot() {
	this->_name = "Vlad";
}

Bot::~Bot() {}

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result;
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    try {
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            result += buffer;
        }
    } catch (std::exception& e) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

std::string Bot::parseWeatherResponse(const std::string& response) {
    size_t key_pos, start, end;

    // Find city name
    key_pos = response.find("\"name\"");
    start = response.find(":", key_pos);
    end = response.find(",", start);
    std::string city = response.substr(start + 1, end - start - 1);
    city.erase(std::remove(city.begin(), city.end(), '\"'), city.end());
    
    // Find weather description
    key_pos = response.find("\"description\"");
    start = response.find(":", key_pos);
    end = response.find(",", start);
    std::string description = response.substr(start + 1, end - start - 1);
    description.erase(std::remove(description.begin(), description.end(), '\"'), description.end());

    // Find temperature
    key_pos = response.find("\"temp\"");
    start = response.find(":", key_pos);
    end = response.find(",", start);
    std::string temp_str = response.substr(start + 1, end - start - 1);
    double temperature = std::strtod(temp_str.c_str(), NULL) - 273.15; // Convert from Kelvin to Celsius

	char *temp = ft_ftoa(temperature);

	std::string toRet = "Weather in " + city + ": " + description + ", Temperature: " + temp + "°C";
	delete [] temp;
    return toRet;
}

std::string Bot::getWeather(std::string city) {
    std::string api_key = "f8256d6bf00097afab7928c620d8af85";
    std::string url = "curl -s \"http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + api_key + "\"";

    std::string readBuffer;
	try {
		readBuffer = exec(url.c_str());
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
    return readBuffer;
}

void Bot::respondToCommand(std::string city, std::string channel, Client& from) {
    std::string weather = getWeather(city);
	if (weather == "{\"cod\":\"404\",\"message\":\"city not found\"}")
	{
		std::string message = ":VladTheRobot!VladTheRobot@server PRIVMSG " + channel + " :City not found.\r\n";
		send(from.getFdClient(), message.c_str(), message.size(), 0);
		return ;
	}
	std::string parsedMsg = parseWeatherResponse(weather);
	if (weather.empty()) {
		weather = "Error: Unable to get weather data";
	}

    std::string message = ":VladTheRobot!VladTheRobot@server PRIVMSG " + channel + " :" + parsedMsg + "\r\n";
    send(from.getFdClient(), message.c_str(), message.size(), 0);
}

void	Server::sendWeatherRequest(std::string &buff, int fdSender) {
	std::deque<std::string> data = splitBuffer(buff, ' ');
	Client from = findClientWithFd(fdSender);
	if (data.size() != 4){
		std::string message = ":VladTheRobot!VladTheRobot@server PRIVMSG " +  data[1] + " :Invalid command.\r\n";
		servSendMessageToClient(message, from);
		return ;}
	Bot bot;
	bot.respondToCommand(data[3], data[1], from);
	(void)fdSender;
}
