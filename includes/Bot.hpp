#ifndef BOT_HPP
# define BOT_HPP

# include "Channel.hpp"
# include "Client.hpp"
# include <string>
# include <iostream>

class Client;
class Bot {
	private:

	std::string _name;
	std::string getWeather(std::string city);
	std::string parseWeatherResponse(const std::string& response);

	public :

	Bot();
	~Bot();
	void	respondToCommand(std::string city, std::string channel, Client& from);
};



#endif 