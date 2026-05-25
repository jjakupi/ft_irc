#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <cstdlib>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
//# include <bits/stdc++.h>
# include <poll.h>
# include <vector>
# include "../Client/Client.hpp"

class Server
{
	private:
		int			_clientFd;
		int			_serverFd;
		int			_port;
		std::string	_password;
		std::vector<Client>	_clients;

	public:
		Server();
		Server(int port, std::string password);
		Server(Server const &oth);
		Server &operator=(Server const &oth);
		~Server();

		int		initSocket();
		int		run();

		Client	*getClient(int fd);
		bool	nickNameAlreadyUsed(std::string nickname);
		void	sendResponse(int fd, std::string message);
};

#endif
