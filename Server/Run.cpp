#include "Server.hpp"

int	Server::run()
{
	while (true)
	{
		int ret = poll(_pollFds.data(), _pollFds.size(), -1);
		if (ret == -1)
		{
			perror("poll");
			return (-1);
		}

		for (size_t i = 0; i < _pollFds.size(); i++)
		{
			if (_pollFds[i].revents & POLLIN)
			{
				if (_pollFds[i].fd == _serverFd)
				{
					int clientFd = accept(_serverFd, NULL, NULL);
					if (clientFd == -1)
					{
						perror("accept");
						continue;
					}

					std::cout << "New client connected, fd = "
							  << clientFd << std::endl;

					pollfd clientPoll;
					clientPoll.fd = clientFd;
					clientPoll.events = POLLIN;
					clientPoll.revents = 0;

					_pollFds.push_back(clientPoll);
				}
				else
				{
					std::vector<char> buf(5000);
					int bytes = recv(_pollFds[i].fd, buf.data(), buf.size(), 0);

					if (bytes > 0)
					{
						std::string msg(buf.data(), bytes);
						std::cout << "Client " << _pollFds[i].fd
								  << " sent: " << msg << std::endl;
					}
					else if (bytes == 0)
					{
						std::cout << "Client " << _pollFds[i].fd
								  << " disconnected" << std::endl;

						close(_pollFds[i].fd);
						_pollFds.erase(_pollFds.begin() + i);
						i--;
					}
					else
					{
						perror("recv");
						close(_pollFds[i].fd);
						_pollFds.erase(_pollFds.begin() + i);
						i--;
					}
				}
			}
		}
	}
	return (0);
}
