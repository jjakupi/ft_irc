#include "Channel.hpp"

Channel::Channel()
{
	this->_channelName = "";
	this->_topic = "";
	this->_hasTopic = false;
}

Channel::Channel(std::string channelname)
	: _channelName(channelname), _topic(""), _hasTopic(false)
{
}

Channel::Channel(const Channel&src)
{
	*this = src;
}

Channel &Channel::operator=(const Channel&src)
{
	if (this != &src)
	{

		this->_channelName = src._channelName;
		this->_users = src._users;
		this->_operators = src._operators;
		this->_topic = src._topic;
		this->_hasTopic = src._hasTopic;
	}
	return *this;
}

Channel::~Channel()
{

}

std::string Channel::getChannelName() const
{
	return this->_channelName;
}

void Channel::setChannel(std::string name) {
	this->_channelName = name;
}

bool	Channel::hasUser(int fd) const
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		if (_users[i] == fd)
			return (true);
	}
	return (false);
}

bool	Channel::isOperator(int fd) const
{
	for (size_t i = 0; i < _operators.size(); i++)
	{
		if (_operators[i] == fd)
			return (true);
	}
	return (false);
}

void	Channel::addUser(int fd)
{
	if (!hasUser(fd))
		_users.push_back(fd);
}

void	Channel::addOperator(int fd)
{
	if (!isOperator(fd))
		_operators.push_back(fd);
}

std::vector<int>	Channel::getUsers() const
{
	return (_users);
}

bool	Channel::hasTopic() const
{
	return (_hasTopic);
}

std::string Channel::getTopic() const
{
	return this->_topic;
}

void Channel::setTopic(std::string topic)
{
	this->_topic = topic;
	this->_hasTopic = true;
}
