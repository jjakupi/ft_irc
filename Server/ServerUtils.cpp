#include "Server.hpp"
#include "../Client/Client.hpp"

Client *Server::getClient(int fd) {
	for (size_t i; i < _clients.size(); i++) {
		if (_clients[i] == fd)
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
