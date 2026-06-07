#include "../Server/Server.hpp"

void	Server::invite(std::string cmd, int fd)
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
	if (args.size() < 3)
	{
		sendResponse(fd, ERR_NEEDMOREPARAMS(cmd));
		return ;
	}

	std::string targetNick = args[1];
	if (!isValidNickName(targetNick)) {
		sendResponse(fd, ERR_ERRONEUSNICK(targetNick));
		return ;
	}
	Client *targetClient = getClientNick(targetNick);
	if (!targetClient)
	{
		sendResponse(fd, ERR_NOSUCHNICK(client->getNickName(), targetNick));
		return ;
	}

	std::string channelName = args[2];
	if (!isValidChannelName(channelName))
	{
		sendResponse(fd, ERR_BADCHANMASK(client->getNickName(), channelName));
		return ;
	}
	Channel *channel = getChannel(channelName);
	if(!channel)
	{
		sendResponse(fd, ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
		return;
	}
	if (!channel->hasUser(fd))
	{
		sendResponse(fd, ERR_NOTONCHANNEL(client->getNickName(), channelName));
		return ;
	}
	if (channel->hasUser(targetClient->getFd()))
	{
		sendResponse(fd, ERR_USERONCHANNEL(client->getNickName(), targetNick, channelName));
		return ;
	}
	if(channel->isInvitedOnly())
	{
		if(!channel->isOperator(fd))
		{
			sendResponse(fd, ERR_CHANOPRIVSNEEDED(client->getNickName(), channelName));
			return ;
		}
		channel->addInvitedUser(targetClient->getFd());
	}
	sendResponse(fd, RPL_INVITING(client->getNickName(), channelName, targetNick));
	sendResponse(targetClient->getFd(), RPL_INVITE(client->getNickName(), client->getUserName(), targetNick, channelName));
}
