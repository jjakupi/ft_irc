#include "Server.hpp"
#include "../Client/Client.hpp"

Client *Server::getClient(int fd) {
	for (size_t i; i < _clients.size(); i++) {
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
		std::cout << "Client " << fd << " sent: " << msg << std::endl;

		// later:
		//Client *client = Fd(fd);
		// client->appendBuffer(msg);
		// extract commands
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
}
