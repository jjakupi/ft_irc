#include "Client.hpp"

Client::Client() {
	this->_fd = -1;
	this->_nickName = "";
	this->_userName = "";
	this->_registered = false;
	this->_loggedIn = false;
}

Client::Client(std::string nickname, std::string username, int fd) : _nickName(nickname), _userName(username), _fd(fd) {
	this->_registered = false;
	this->_loggedIn = false;
}

Client::Client(const Client &src) {
	*this = src;
}

Client& Client::operator=(const Client &src) {
		if (this != &src) {
		this->_fd = src._fd;
		this->_nickName = src._nickName;
		this->_userName = src._userName;
		this->_registered = src._registered;
		this->_loggedIn = src._loggedIn;
	}
	return *this;
}

Client::~Client() {}

int Client::getFd() const {
	return this->_fd;
}

std::string Client::getNickName() const {
	return this->_nickName;
}

std::string Client::getUserName() const {
	return this->_userName;
}

bool Client::getRegistered() const {
	return this->_registered;
}

bool Client::getLoggedIn() const {
	return this->_loggedIn;
}

void Client::setFd(int fd) {
	this->_fd = fd;
}

void Client::setNickName(std::string nickname) {
	this->_nickName = nickname;
}

void Client::setUserName(std::string username) {
	this->_userName = username;
}

void Client::setRegistered(bool value) {
	this->_registered = value;
}

void Client::setLoggedIn(bool value) {
	this->_loggedIn = value;
}
