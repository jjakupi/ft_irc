#include "../Server/Server.hpp"

void	Server::join(std::string cmd, int fd)
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
		sendResponse(fd, ERR_NEEDMOREPARAMS("JOIN"));
		return ;
	}

	std::vector<std::string> channels = splitByComma(args[1]);
	std::vector<std::string> keys;

	if (args.size() >= 3)
		keys = splitByComma(args[2]);

	for (size_t i = 0; i < channels.size(); i++)
	{
		std::string key = "";
		if (i < keys.size())
			key = cleanKey(keys[i]);
		joinOneChannel(client, fd, channels[i], key);
		if (!getClient(fd))
			return ;
	}
}
void	Server::joinOneChannel(Client *client, int fd, std::string channelName, std::string key)
{
	std::string originalName = channelName;
	std::string searchName = normalizeChannelName(channelName);

	if (!isValidChannelName(channelName))
	{
		sendResponse(fd, ERR_BADCHANMASK(client->getNickName(), channelName));
		return ;
	}

	Channel *channel = getChannel(searchName);

	if (!channel)
	{
		Channel newChannel(originalName);
		newChannel.addUser(fd);
		newChannel.addOperator(fd);
		_channels.push_back(newChannel);
		channel = getChannel(searchName);
		if (!channel)
			return ;
	}
	else
	{
		if (channel->hasUser(fd))
		{
			sendResponse(fd, ERR_USERONCHANNEL(client->getNickName(),
				client->getNickName(), originalName));
			return ;
		}
		if (channel->isInvitedOnly() && !channel->hasInvitedUsers(fd))
		{
			sendResponse(fd, ERR_INVITEONLYCHAN(client->getNickName(), originalName));
			return ;
		}
		std::vector<int> users = channel->getUsers();
		if (channel->hasLimit() && users.size() >= channel->getLimit())
		{
			sendResponse(fd, ERR_CHANNELISFULL(client->getNickName(), originalName));
			return ;
		}
		if (channel->hasKey())
		{
			if (key.empty() || !isValidChannelKey(key) || key != channel->getKey())
			{
				sendResponse(fd, ERR_BADCHANNELKEY(client->getNickName(), originalName));
				return ;
			}
		}
		channel->addUser(fd);
	}
	std::vector<int> users = channel->getUsers();
	std::string joinMsg = RPL_JOIN(client->getNickName(), client->getUserName(), originalName);

	for (size_t i = 0; i < users.size(); i++)
		sendResponse(users[i], joinMsg);

	std::string names = getChannelNames(channel);
	sendResponse(fd, RPL_NAMREPLY(client->getNickName(), originalName, names));
	sendResponse(fd, RPL_ENDOFNAMES(client->getNickName(), originalName));
}
