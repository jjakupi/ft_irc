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
		std::vector<int>	_invitedUsers;
		bool				_invitedOnly;
		size_t				_Limit;
		bool				_hasLimit;
		bool				_hasKey;
		std::string			_key;

	public:
		Channel();
		Channel(std::string channelname);
		Channel(const Channel &src);
		Channel &operator=(const Channel &src);
		~Channel();

	std::vector<int>	getUsers() const;
	std::string 		getChannelName() const;
	void 				setChannel(std::string name);
	void				addUser(int fd);
	void				addOperator(int fd);
	void				removeOperator(int fd);
	bool				hasUser(int fd) const;
	bool				hasOperators() const;
	bool				isOperator(int fd) const;
	bool				isValidChannelName(std::string channelName);
	void				removeUser(int fd);
	//topic
	bool				hasTopic() const;
	std::string 		getTopic() const;
	void				setTopic(std::string topic);
	//invite
	bool				hasInvitedUsers(int fd) const;
	std::vector<int>	getInvitedUsers() const;
	void				addInvitedUser(int fd);
	bool				isInvitedOnly() const;
	void 				setInviteOnly(bool value);
	//limit
	bool				hasLimit() const;
	size_t				getLimit() const;
	void				setHasLimit(bool value);
	void				setLimit(size_t limit);
	//key
	bool 				hasKey() const;
	std::string 		getKey() const;
	void				setKey(std::string key);
	void 				removeKey();
};

#endif
