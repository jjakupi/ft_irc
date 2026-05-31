#include "../Server/Server.hpp"
#include "Replies.hpp"
#include "../Client/Client.hpp"

Client *Server::getClient(int fd) {
	for (size_t i = 0; i < _clients.size(); ++i) {
		if (_clients[i].getFd() == fd)
			return (&_clients[i]);
	}
	return NULL;
}

bool Server::nickNameAlreadyUsed(const std::string &nickname) {
	for (size_t i = 0; i < _clients.size(); ++i) {
		if (_clients[i].getNickName() == nickname)
			return true;
	}
	return false;
}

void Server::sendResponse(int fd, std::string message)
{
	send(fd, message.c_str(), message.size(), 0);
}

void Server::addClient(int fd)
{
	Client newClient;
	newClient.setFd(fd);
	_clients.push_back(newClient);
}

/*
Selon la RFC 2812 :
Le premier caractère doit être : (a-z, A-Z) [ ] \ ` _ ^ { | }
Les caractères suivants peuvent être : (a-z, A-Z) (0-9) [ ] \ ` _ ^ { | } -
*/
bool Server::isValidNickName(std::string nickname)
{
	if(nickname.empty())
		return false;
	if (!((nickname[0] >= 65 && nickname[0] <= 122) || (nickname[0] >= 91 && nickname[0] <= 96) || (nickname[0] >= 123 && nickname[0] <= 125)))
		return false;
	for (size_t i = 1; i < nickname.size(); i++) {
		if (!(isalnum(nickname[i]) || (nickname[i] >= 91 && nickname[i] <= 96) || (nickname[i] >= 123 && nickname[i] <= 125) || (nickname[i] == 45)))
			return false;
	}
	return (true);
}

bool Server::isValidUserName(std::string username) {
	if(username.empty())
		return false;
	for (size_t i = 0; i < username.size(); i++) {
		if ((username[i] >= 0 && username[i] <= 32) || username[i] == 64)
			return false;
	}
	return (true);
}

std::vector<std::string>	Server::splitCommand(std::string cmd)
{
	std::vector<std::string>	args;
	size_t					i;
	size_t					start;

	i = 0;
	while (i < cmd.size())
	{
		while (i < cmd.size() && cmd[i] == ' ')
			i++;

		if (i >= cmd.size())
			break ;

		if (cmd[i] == ':')
		{
			args.push_back(cmd.substr(i + 1));
			break ;
		}

		start = i;
		while (i < cmd.size() && cmd[i] != ' ')
			i++;

		args.push_back(cmd.substr(start, i - start));
	}
	return (args);
}

void	Server::tryRegister(Client *client, int fd)
{
	if (!client)
		return ;

	if (client->getRegistered()
		&& !client->getNickName().empty()
		&& !client->getUserName().empty()
		&& !client->getLoggedIn())
	{
		client->setLoggedIn(true);
		sendResponse(fd, RPL_WELCOME(client->getNickName(), client->getUserName()));
	}
}

bool	Server::isValidChannelName(std::string channelName)
{
	if (channelName.empty())
		return (false);
	if (channelName.size() < 2)
		return (false);
	if (channelName.size() > 50)
		return (false);
	if (channelName[0] != '#' && channelName[0] != '&'
		&& channelName[0] != '+' && channelName[0] != '!')
		return (false);
	if (channelName.find(':') != std::string::npos)
		return (false);
	if (channelName.find(' ') != std::string::npos)
		return (false);
	if (channelName.find('\a') != std::string::npos)
		return (false);
	if (channelName.find('\t') != std::string::npos)
		return (false);
	if (channelName.find(',') != std::string::npos)
		return (false);
	return (true);
}

std::string Server::normalizeChannelName(std::string channelName)
{
	for(size_t i = 0 ; i < channelName.size(); i++)
	{
		channelName[i] = std::toupper(channelName[i]);
	}
	return channelName;
}

std::string	Server::getChannelNames(Channel *channel)
{
	std::vector<int>	users;
	std::string			names;
	Client				*client;

	if (!channel)
		return ("");

	users = channel->getUsers();
	for (size_t i = 0; i < users.size(); i++)
	{
		client = getClient(users[i]);
		if (!client)
			continue ;

		if (!names.empty())
			names += " ";

		if (channel->isOperator(users[i]))
			names += "@";

		names += client->getNickName();
	}
	return (names);
}
