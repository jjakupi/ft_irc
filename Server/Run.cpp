#include "Server.hpp"

extern volatile sig_atomic_t g_running;

int	Server::run()
{
	while (g_running)
	{
		errno = 0;
		int ret = poll(&_pollFds[0], _pollFds.size(), -1);

		if (ret == -1)
		{
			std::cout << "poll failed, errno = " << errno << std::endl;

			if (errno == EINTR)
				break ;

			perror("poll");
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
