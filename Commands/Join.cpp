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
			std::cout << "fd " << fd << " is already in channel" << std::endl;
			return ;
		}
		channel->addUser(fd);
		std::cout << "fd " << fd << " added as USER only" << std::endl;
	}
		std::cout << "Total channels = " << _channels.size() << std::endl;
		std::cout << "Channel Name = " << channelName << std::endl;

		std::vector<int> users = channel->getUsers();

		for (size_t i = 0; i < users.size(); i++)
		{
			sendResponse(users[i], RPL_JOIN(client->getNickName(), client->getUserName(), channelName));
		}
		std::string names = getChannelNames(channel);
		sendResponse(fd, RPL_NAMREPLY(client->getNickName(), channelName, names));
		sendResponse(fd, RPL_ENDOFNAMES(client->getNickName(), channelName));
}

