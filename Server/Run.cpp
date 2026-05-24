#include "Server.hpp"

int Server::run()
{
	while(true)
	{
		int acceptFd = accept(_serverFd, NULL, NULL);
		if(acceptFd == -1)
		{
			perror("accept");
			close(_serverFd);
			_serverFd = -1;
			return (-1);
		}
	}
}
