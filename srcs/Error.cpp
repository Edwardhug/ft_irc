#include "../includes/Error.hpp"

void sendErrorToClient(const std::string &message, Client &client)
{
    send(client.getFdClient(), message.c_str(), message.length(), 0);
}