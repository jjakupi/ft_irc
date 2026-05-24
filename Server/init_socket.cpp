#include "server.hpp"

int	Server::initSocket()
{
	m_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_serverFd == -1)
	{
		perror("socket");
		return (-1);
	}
	
	int opt = 1;
	if(setsockopt(m_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		perror("setsockopt");
		close(m_serverFd);
		m_serverFd = -1;
		return (-1);
	}
	sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(m_port);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(m_serverFd, (struct sockaddr *)&serverAddress,sizeof(serverAddress)) == -1)
	{
		perror("bind");
		close(m_serverFd);
		m_serverFd = -1;
		return (-1);
	}

	if (listen(m_serverFd, 10) == -1)
	{
		perror("listen");
		close(m_serverFd);
		m_serverFd = -1;
		return (-1);
	}

	std::cout << "Server listening on port " << m_port << std::endl;
	return (0);
}
