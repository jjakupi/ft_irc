#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>

class Client {
	private:
		int			_fd;
		std::string	_nickName;
		std::string	_userName;
		bool		_registered;
		bool		_loggedIn;
		std::string	_buffer;

	public:
		Client();
		Client(int fd);
		Client(std::string nickname, std::string username, int fd);
		Client(const Client &src);
		Client &operator=(const Client &src);
		~Client();

		int			getFd() const;
		std::string	getNickName() const;
		std::string	getUserName() const;
		bool		getRegistered() const;
		bool		getLoggedIn() const;

		void	setFd(int fd);
		void	setNickName(std::string nickname);
		void	setUserName(std::string username);
		void	setRegistered(bool value);
		void	setLoggedIn(bool value);
		void	appendBuffer(const std::string &data);
		bool	hasCompleteCommand() const;
		std::string	popCommand();
};

#endif
