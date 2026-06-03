#include "../Server/Server.hpp"
#include "Replies.hpp"
#include "../Client/Client.hpp"
#include "../Channel/Channel.hpp"

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

	for (size_t i = 0; i < args.size(); i++)
		std::cout << "args[" << i << "]=[" << args[i] << "]" << std::endl;
	if (args.size() < 2)
	{
		sendResponse(fd, ERR_NEEDMOREPARAMS(cmd));
		return ;
	}

	std::string channelName = args[1];
	if (!isValidChannelName(channelName))
	{
		sendResponse(fd, ERR_BADCHANMASK(channelName));
		return;
	}
	channelName = normalizeChannelName(channelName);
	Channel *channel = getChannel(channelName);

	if (!channel)
	{
		Channel newChannel(channelName);

		newChannel.addUser(fd);
		newChannel.addOperator(fd);
		std::cout << "fd " << fd << " added as USER and OPERATOR" << std::endl;
		_channels.push_back(newChannel);
		client->getUserName();
		client->getNickName();
		channel = getChannel(channelName);

	}
	else
	{
		if (channel->hasUser(fd))
		{
			sendResponse(fd, ERR_USERONCHANNEL(client->getNickName(), channelName));
			return ;
		}
		if(channel->isInvitedOnly() && (!channel->hasInvitedUsers(fd)))
		{
			sendResponse(fd, ERR_INVITEONLYCHAN(client->getNickName(), channelName));
			return;
		}
		std::vector<int> users = channel->getUsers();
		if (channel->hasLimit() && users.size() >= channel->getLimit())
		{
			sendResponse(fd, ERR_CHANNELISFULL(client->getNickName(), channelName));
			return ;
		}
		else
			channel->addUser(fd);
	}
	std::cout << "fd " << fd << " added as USER only" << std::endl;
	std::cout << "Total channels = " << _channels.size() << std::endl;
	std::cout << "Channel Name = " << channelName << std::endl;
	std::vector<int> users = channel->getUsers();
	std::cout << "Total users = " << users.size() << "in channel =" << channelName << std::endl;
	for (size_t i = 0; i < users.size(); i++)
	{
		sendResponse(users[i], RPL_JOIN(client->getNickName(), client->getUserName(), channelName));
	}
	std::string names = getChannelNames(channel);
	sendResponse(fd, RPL_NAMREPLY(client->getNickName(), channelName, names));
	sendResponse(fd, RPL_ENDOFNAMES(client->getNickName(), channelName));
}

