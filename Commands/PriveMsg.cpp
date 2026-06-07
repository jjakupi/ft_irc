#include "../Server/Server.hpp"

bool	isChannelTarget(std::string target)
{
	if (target[0] != '#' && target[0] != '&' && target[0] != '+' && target[0] != '!')
		return false;
	return true;
}

Client *Server::getClientNick(std::string nickName)
{
	for (size_t i = 0; i < _clients.size(); ++i) {
		if (_clients[i].getNickName() == nickName)
			return (&_clients[i]);
	}
	return NULL;
}


void Server::privmsg(std::string cmd, int fd)
{
	Client *client = getClient(fd);
	if (!client)
		return ;
	if (!client->getLoggedIn())
	{
		sendResponse(fd, ERR_NOTREGISTERED(client->getNickName()));
		return ;
	}
	std::vector<std::string> args = splitCommand(cmd);
	if (args.size() < 2)
	{
		sendResponse(fd, ERR_NORECIPIENT(client->getNickName(), "PRIVMSG"));
		return ;
	}
	if (args.size() < 3)
	{
		sendResponse(fd, ERR_NOTEXTTOSEND(client->getNickName()));
		return ;
	}
	std::string target = args[1];
	if(isChannelTarget(target))
	{
		std::string channelName = args[1];

		Channel *channel = getChannel(channelName);
		if (!isValidChannelName(channelName))
		{
			sendResponse(fd, ERR_BADCHANMASK(client->getNickName(), channelName));
			return ;
		}
		if(!channel)
		{
			sendResponse(fd, ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
			return;
		}
		if (!channel->hasUser(fd))
		{
			sendResponse(fd, ERR_CANNOTSENDTOCHAN(client->getNickName(), channelName));
			return ;
		}
		std::string message = args[2];
		std::vector<int> users = channel->getUsers();
		for (size_t i = 0; i < users.size(); i++)
		{
			if(users[i] == fd)
				continue;
			sendResponse(users[i], RPL_PRIVMSG(client->getNickName(), client->getUserName(), channelName, message));
		}
	}
	else
	{
		std::string nickName = args[1];
		if (!isValidNickName(nickName))
		{
			sendResponse(fd, ERR_ERRONEUSNICK(nickName));
			return ;
		}
		Client *clientNick = getClientNick(nickName);
		if (!clientNick)
		{
			sendResponse(fd, ERR_NOSUCHNICK(client->getNickName(), nickName));
			return ;
		}
		std::string message = args[2];
		sendResponse(clientNick->getFd(), RPL_PRIVMSG(client->getNickName(), client->getUserName(), nickName, message));
	}
}
