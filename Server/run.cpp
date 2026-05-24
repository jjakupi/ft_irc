#include "server.hpp"

int Server::run()
{
	while(true)
	{
		int acceptFd = accept(m_serverFd, NULL, NULL);
		if(acceptFd == -1)
		{
			perror("accept");
			close(m_serverFd);
			m_serverFd = -1;
			return (-1);
		}
	}
}
