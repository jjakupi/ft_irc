#include "Server.hpp"

Server::Server()
	: _serverFd(-1), _port(0), _password("")
{
}

Server::Server(int port, std::string password)
	: _serverFd(-1), _port(port), _password(password)
{
}

Server::Server(Server const &oth)
	: _serverFd(oth._serverFd),
	  _port(oth._port),
	  _password(oth._password)
{
}

Server &Server::operator=(Server const &oth)
{
	if (this != &oth)
	{
		_serverFd = oth._serverFd;
		_port = oth._port;
		_password = oth._password;
	}
	return (*this);
}

Server::~Server()
{
	if (_serverFd != -1)
		close(_serverFd);
}

void	Server::cleanup()
{
	for (size_t i = 0; i < _pollFds.size(); i++)
	{
		if (_pollFds[i].fd >= 0)
			close(_pollFds[i].fd);
	}

	_pollFds.clear();
	_clients.clear();
	_channels.clear();
}
