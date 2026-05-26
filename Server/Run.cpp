#include "Server.hpp"

int Server::run()
{
	while (true)
	{
		if (poll(_pollFds.data(), _pollFds.size(), -1) == -1)
		{
			perror("poll");
			return (-1);
		}

		for (size_t i = 0; i < _pollFds.size(); i++)
		{
			if (_pollFds[i].revents & POLLIN)
			{
				if (_pollFds[i].fd == _serverFd)
					acceptClient();
				else
					receiveFromClient(_pollFds[i].fd);
			}
		}
	}
	return (0);
}
