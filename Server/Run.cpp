#include "Server.hpp"

int	Server::run()
{
	while (Server::isRunning())
	{
		int ret = poll(&_pollFds[0], _pollFds.size(), -1);

		if (ret == -1)
		{
			if (!Server::isRunning())
				break ;
			std::cerr << "poll error" << std::endl;
			return (-1);
		}

		for (size_t i = 0; i < _pollFds.size(); i++)
		{
			if (_pollFds[i].revents & POLLIN)
			{
				int currentFd = _pollFds[i].fd;

				if (currentFd == _serverFd)
					acceptClient();
				else
					receiveFromClient(currentFd);
			}
		}
	}
	return (0);
}
