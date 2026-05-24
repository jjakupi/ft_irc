#include "Server.hpp"

Server::Server()
	: m_serverFd(-1), m_port(0), m_password("")
{
}

Server::Server(int port, std::string password)
	: m_serverFd(-1), m_port(port), m_password(password)
{
}

Server::Server(Server const &oth)
	: m_serverFd(oth.m_serverFd),
	  m_port(oth.m_port),
	  m_password(oth.m_password)
{
}

Server &Server::operator=(Server const &oth)
{
	if (this != &oth)
	{
		m_serverFd = oth.m_serverFd;
		m_port = oth.m_port;
		m_password = oth.m_password;
	}
	return (*this);
}

Server::~Server()
{
	if (m_serverFd != -1)
		close(m_serverFd);
}
