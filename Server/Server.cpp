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

volatile sig_atomic_t	Server::_running = 1;

void	Server::handleSignal(int sig)
{
	(void)sig;
	Server::_running = 0;
}

bool	Server::isRunning()
{
	return (Server::_running != 0);
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

void Server::ping(std::string cmd, int fd)
{
	std::vector<std::string> args = splitCommand(cmd);

	if (args.size() >= 2)
		sendResponse(fd, "PONG " + args[1] + "\r\n");
	else
		sendResponse(fd, "PONG :server\r\n");
}
void Server::cap(std::string cmd, int fd)
{
	std::vector<std::string> args = splitCommand(cmd);

	if (args.size() < 2)
		return ;

	if (args[1] == "LS")
	{
		sendResponse(fd, ":server CAP * LS :\r\n");
		return ;
	}
	else if (args[1] == "REQ")
	{
		// WeeChat requests capabilities.
		// Since we support none, we can ACK the empty request.
		sendResponse(fd, ":server CAP * ACK :\r\n");
		return ;
	}
	else if (args[1] == "END")
	{
		// End of CAP negotiation. Do nothing.
		return ;
	}
}
