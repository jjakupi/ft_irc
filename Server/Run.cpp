#include "Server.hpp"

extern volatile sig_atomic_t g_running;

int Server::run()
{
	while (g_running)
	{
		if (poll(_pollFds.data(), _pollFds.size(), -1) == -1)
		{
			if (errno == EINTR)
				break ;
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
