#include "../includes/Bot.hpp"
#include "../includes/Server.hpp"
#include "../includes/lib.hpp"
#include <curl/curl.h>

Bot::Bot() {
	this->_name = "Vlad";
}

Bot::~Bot() {}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

std::string Bot::getWeather(std::string city) {
    std::string api_key = "f8256d6bf00097afab7928c620d8af85";
    std::string url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + api_key;

    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            return "Error: Unable to get weather data";
        }
    }
    return readBuffer;
}

void Bot::respondToCommand(std::string city, std::string channel, Client& from) {
    std::string weather = getWeather(city);
	std::cout<< GREEN << weather << RESET << std::endl;

    std::string message = ":Server PRIVMSG " + channel + " :" + weather + "\r\n";
    send(from.getFdClient(), message.c_str(), message.size(), 0);
	(void)from;
}

void	Server::sendWeatherRequest(std::string &buff, int fdSender) {
	std::deque<std::string> data = splitBuffer(buff, ' ');
	// Channel channel = findChannelWithName(data[1]); // data[1] = #channel
	Client from = findClientWithFd(fdSender);
	if (data.size() != 4){
		std::string message = ":Server NOTICE " +  data[1] + " :Invalid command.\r\n";
		servSendMessageToClient(message, from);
		return ;}
	Bot bot;
	// std::cout << GREEN << data[3] << "."<< RESET << std::endl;
	bot.respondToCommand(data[3], data[0], from);
	(void)fdSender;
}