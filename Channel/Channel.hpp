#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <algorithm>
# include <vector>

class Channel
{
	private:
		std::string			_channelName;
		std::vector<int>	_users;
		std::vector<int>	_operators;
		std::string			_topic;
		bool				_hasTopic;

	public:
		Channel();
		Channel(std::string channelname);
		Channel(const Channel &src);
		Channel &operator=(const Channel &src);
		~Channel();

	std::vector<int>	getUsers() const;
	std::string 		getChannelName() const;
	std::string 		getTopic() const;
	void 				setChannel(std::string name);
	void				addUser(int fd);
	void				addOperator(int fd);
	bool				hasUser(int fd) const;
	bool				hasOperator(int fd) const;
	bool				isOperator(int fd) const;
	bool				isValidChannelName(std::string channelName);
	void				removeUser(int fd);
	bool				hasTopic() const;
	void				setTopic(std::string topic);
};

#endif
