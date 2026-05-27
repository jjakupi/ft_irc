#include "Server.hpp"
#include "../Client/Client.hpp"

Client *Server::getClient(int fd) {
<<<<<<< HEAD
<<<<<<< HEAD
	for (size_t i = 0; i < _clients.size(); i++) {
=======
	for (size_t i; i < _clients.size(); i++) {
>>>>>>> f32decf (serverUtils)
=======
	for (size_t i = 0; i < _clients.size(); i++) {
>>>>>>> 902fe50 (parsed the cleancommand)
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
<<<<<<< HEAD
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
=======

void	Server::acceptClient()
{
	{
		int clientFd = accept(_serverFd, NULL, NULL);
		if (clientFd == -1)
		{
			perror("accept");
				return;
		}
		std::cout << "New client connected, fd = "<< clientFd << std::endl;
		pollfd clientPoll;
		clientPoll.fd = clientFd;
		clientPoll.events = POLLIN;
		clientPoll.revents = 0;
		_pollFds.push_back(clientPoll);
		_clients.push_back(Client(clientFd));
	}
}

void	Server::receiveFromClient(int fd)
{
	std::vector<char> buf(5000);
	int bytes = recv(fd, buf.data(), buf.size(), 0);

	if (bytes > 0)
	{
		std::string msg(buf.data(), bytes);
		//std::cout << "Client " << fd << " sent: " << msg << std::endl;

		// later:
		Client *client = getClient(fd);
		client->appendBuffer(msg);
		while (client->hasCompleteCommand())
		{
			std::string command = client->popCommand();
			command = client->cleanCommand(command);
			if (!command.empty())
				std::cout << "COMMAND=[" << command << "] from client: " << fd << std::endl;
		}

	}
	else if (bytes == 0)
	{
		std::cout << "Client " << fd << " disconnected" << std::endl;
		removeClient(fd);
	}
	else
	{
		perror("recv");
		removeClient(fd);
	}
}

void	Server::removeClient(int fd)
{
	close(fd);

	for (size_t i = 0; i < _pollFds.size(); i++)
	{
		if (_pollFds[i].fd == fd)
		{
			_pollFds.erase(_pollFds.begin() + i);
			break;
		}
	}

	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getFd() == fd)
		{
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
>>>>>>> f32decf (serverUtils)
}
