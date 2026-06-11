#include "../Server/Server.hpp"

void Server::names(std::string cmd, int fd)
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
		// Simple version: if no channel given, just end.
		sendResponse(fd, RPL_ENDOFNAMES(client->getNickName(), "*"));
		return ;
	}

	std::string channelName = args[1];

	Channel *channel = getChannel(channelName);
	if (!channel)
	{
		sendResponse(fd, RPL_ENDOFNAMES(client->getNickName(), channelName));
		return ;
	}

	std::string namesList = getChannelNames(channel);

	sendResponse(fd, RPL_NAMREPLY(client->getNickName(), channelName, namesList));
	sendResponse(fd, RPL_ENDOFNAMES(client->getNickName(), channelName));
}
