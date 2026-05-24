#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <cstdlib>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
#include <bits/stdc++.h>
#include <poll.h>

class Server
{
	private:
		int			m_clientFd;
		int			m_serverFd;
		int			m_port;
		std::string	m_password;

	public:
		Server();
		Server(int port, std::string password);
		Server(Server const &oth);
		Server &operator=(Server const &oth);
		~Server();

		int		initSocket();
		int		run();
};

#endif
