#include "Server.hpp"

int	Server::initSocket()
{
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd == -1)
	{
		perror("socket");
		return (-1);
	}

	int opt = 1;
	if(setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		perror("setsockopt");
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
		perror("bind");
		close(_serverFd);
		_serverFd = -1;
		return (-1);
	}

	if (listen(_serverFd, 10) == -1)
	{
		perror("listen");
		close(_serverFd);
		_serverFd = -1;
		return (-1);
	}

	std::cout << "Server listening on port " << _port << std::endl;
	return (0);
}
