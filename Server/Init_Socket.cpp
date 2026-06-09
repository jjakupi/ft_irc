#include "Server.hpp"

int	Server::initSocket()
{
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd == -1)
	{
		std::cerr << "socket error" << std::endl;
		return (-1);
	}

	int opt = 1;
	if(setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		std::cerr << "setsockopt error" << std::endl;
		close(_serverFd);
		_serverFd = -1;
		return (-1);
	}
	sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(_port);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(_serverFd, (struct sockaddr *)&serverAddress,sizeof(serverAddress)) == -1)
	{

		std::cerr << "bind error" << std::endl;
		close(_serverFd);
		_serverFd = -1;
		return (-1);
	}

	if (listen(_serverFd, 10) == -1)
	{
		std::cerr << "listen error" << std::endl;
		close(_serverFd);
		_serverFd = -1;
		return (-1);
	}
	pollfd serverPoll;
	serverPoll.fd = _serverFd;
	serverPoll.events = POLLIN;
	serverPoll.revents = 0;
	_pollFds.push_back(serverPoll);
	std::cout << "Server listening on port " << _port << std::endl;
	return (0);
}
