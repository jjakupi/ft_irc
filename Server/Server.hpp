#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
#include <cstring>
# include <cstdlib>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <poll.h>
# include <vector>
# include <cstdio>
# include <sstream>
# include "../Client/Client.hpp"
# include "../Channel/Channel.hpp"
# include "../Commands/Replies.hpp"

class Server
{
	private:
		int			_clientFd;
		int			_serverFd;
		int			_port;
		std::string	_password;
		std::vector<pollfd> _pollFds;
		std::vector<Client>	_clients;
		std::vector<Channel> _channels;

		void		acceptClient();
		void		receiveFromClient(int fd);
		void		removeClient(int fd);
	public:
		Server();
		Server(int port, std::string password);
		Server(Server const &oth);
		Server &operator=(Server const &oth);
		~Server();

		int		initSocket();
		int		run();

		Client		*getClient(int fd);
		bool		nickNameAlreadyUsed(const std::string &nickname);
		void		sendResponse(int fd, std::string message);
		void 		addClient(int fd);
		void		addChannel(std::string channelName);
		bool		isValidNickName(std::string nickname);
		bool		isValidUserName(std::string username);
		void		getCommand(std::string command, int fd);
		std::vector<std::string>	splitCommand(std::string cmd);
		void		tryRegister(Client *client, int fd);
		Channel		*getChannel(std::string channelName);
		bool		isValidChannelName(std::string channelName);
		std::string	normalizeChannelName(std::string channelName);
		std::string	createJoinMessage(Client *client, std::string channelName);
		std::string	getChannelNames(Channel *channel);
		void pass(std::string cmd, int fd);
		void nick(std::string cmd, int fd);
		void user(std::string cmd, int fd);
		void join(std::string cmd, int fd);
};

#endif
