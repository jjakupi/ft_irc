#include "../Server/Server.hpp"

void	Server::topic(std::string cmd, int fd)
{
	Client *client = getClient(fd);
	if(!client)
		return;
	if (!client->getLoggedIn())
	{
		sendResponse(fd, ERR_NOTREGISTERED(client->getNickName()));
		return ;
	}
	std::vector<std::string> args = splitCommand(cmd);
	if (args.size() < 2)
	{
		sendResponse(fd, ERR_NEEDMOREPARAMS(cmd));
		return ;
	}
	std::string channelName = args[1];
	if (!isValidChannelName(channelName))
	{
		sendResponse(fd, ERR_BADCHANMASK(channelName));
		return ;
	}
	Channel *channel = getChannel(channelName);
	if(!channel)
	{
		sendResponse(fd, ERR_NOSUCHCHANNEL(channelName));
		return;
	}
	if (!channel->hasUser(fd))
	{
		sendResponse(fd, ERR_NOTONCHANNEL(client->getNickName(), channelName));
		return ;
	}

	if(args.size() == 2)
	{
		if (channel->hasTopic())
			sendResponse(fd, RPL_TOPIC(client->getNickName(), channelName, channel->getTopic()));
		else
			sendResponse(fd, RPL_NOTOPIC(client->getNickName(), channelName));
	}
	else
	{
		std::vector<int> users = channel->getUsers();
		std::string topicText = args[2];
		channel->setTopic(topicText);
		for (size_t i = 0; i < users.size(); i++)
		{
			sendResponse(users[i], RPL_TOPIC_CHANGE(client->getNickName(), client->getUserName(), channelName, topicText));
		}
	}
}
