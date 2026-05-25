#include "Server.hpp"
#include "../Client/Client.hpp"

Client *Server::getClient(int fd) {
	for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i].getFd() == fd)
			return (&_clients[i]);
	}
	return NULL;
}

bool Server::nickNameAlreadyUsed(std::string nickname) {
	for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i].getNickName() == nickname) {
			return true;
		}
	}
	return false;
}

void Server::sendResponse(int fd, std::string message)
{
	send(fd, message.c_str(), message.size(), 0);
}

void Server::addClient(int fd)
{
	Client newClient;
	newClient.setFd(fd);
	_clients.push_back(newClient);
}
/*
Selon la RFC 2812 :
Le premier caractère doit être : (a-z, A-Z) [ ] \ ` _ ^ { | }
Les caractères suivants peuvent être : (a-z, A-Z) (0-9) [ ] \ ` _ ^ { | } -
*/
bool Server::isValidNickName(std::string nickname) {
	if(nickname.empty())
		return false;
	if (!((nickname[0] >= 65 && nickname[0] <= 122) || (nickname[0] >= 91 && nickname[0] <= 96) || (nickname[0] >= 123 && nickname[0] <= 125)))
		return false;
	for (size_t i = 0; i < nickname.size(); i++) {
		if (!(isalnum(nickname[i]) || (nickname[i] >= 91 && nickname[i] <= 96) || (nickname[i] >= 123 && nickname[i] <= 125) || (nickname[i] == 45)))
			return false;
	}
	return true;
}
