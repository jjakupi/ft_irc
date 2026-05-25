#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <cstdlib>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <bits/stdc++.h>
# include <sys/poll.h>
class Server
{
	private:
		int					_clientFd;
		int					_serverFd;
		int					_port;
		std::vector<pollfd> _pollFds;
		std::string			_password;

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
