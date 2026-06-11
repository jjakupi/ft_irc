#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <cstdlib>
# include <fcntl.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <poll.h>
# include <vector>
# include <cstdio>
# include <sstream>
# include <csignal>
# include "../Client/Client.hpp"
# include "../Channel/Channel.hpp"
# include "../Commands/Replies.hpp"

#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"

class Server
{
	private:
		int			_serverFd;
		int			_port;
		std::string	_password;
		std::vector<pollfd> _pollFds;
		std::vector<Client>	_clients;
		std::vector<Channel> _channels;

		void		acceptClient();
		void		receiveFromClient(int fd);

		static volatile sig_atomic_t	_running;
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

		//channel
		Channel		*getChannel(std::string channelName);
		bool		isValidChannelName(std::string channelName);
		std::string	normalizeChannelName(std::string channelName);
		std::vector<std::string>	splitByComma(std::string str);
		bool		isValidChannelKey(const std::string &key);
		std::string	cleanKey(std::string key);
		std::string	getChannelNames(Channel *channel);
		bool		isValidModeCommand(std::string command);
		//privmsg
		Client			*getClientNick(std::string nickName);
		void			cleanup();
		static void		handleSignal(int sig);
		static bool		isRunning();

		void cap(std::string cmd, int fd);
		void joinOneChannel(Client *client, int fd, std::string channelName, std::string key);
		void pass(std::string cmd, int fd);
		void nick(std::string cmd, int fd);
		void user(std::string cmd, int fd);
		void join(std::string cmd, int fd);
		void quit(std::string cmd, int fd);
		void privmsg(std::string cmd, int fd);
		void kick(std::string cmd, int fd);
		void invite(std::string cmd, int fd);
		void topic(std::string cmd, int fd);
		void mode(std::string cmd, int fd);
		void ping(std::string cmd, int fd);
};

#endif
