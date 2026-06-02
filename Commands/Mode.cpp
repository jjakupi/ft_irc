#include "../Server/Server.hpp"
#include "Replies.hpp"
#include "../Client/Client.hpp"

bool	Server::isValidModeCommand(std::string command)
{
	std::string cmd = command;
	if(cmd.empty())
		return false;
	if(cmd.size() != 2)
		return false;
	for(size_t i = 0; i < cmd.size(); i++)
	{
	if(cmd[0] != '+' && cmd[0] != '-')
		return false;
	else if(cmd[1] != 'i' && cmd[1] != 't' && cmd[1] != 'k' && cmd[1] != 'o' && cmd[1] != 'l')
		return false;
	}
	return true;
}
void Server::mode(std::string cmd, int fd)
{
	Client *client = getClient(fd);
	if(!client)
		return;
	if(!client->getLoggedIn())
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
	if(!channel->hasUser(fd))
	{
		sendResponse(fd, ERR_NOTONCHANNEL(client->getNickName(), channelName));
		return ;
	}
	if(!channel->isOperator(fd))
	{
		sendResponse(fd, ERR_CHANOPRIVSNEEDED(cmd));
		return ;
	}
	std::string modeChar = args[2];
	if(!isValidModeCommand(modeChar))
	{
		sendResponse(fd, ERR_UNKNOWNMODE(client->getNickName(), modeChar));
		return;
	}
	std::vector<int> users = channel->getUsers();
	if (modeChar.size() >= 2 && modeChar[0] == '+' && modeChar[1] == 'i')
	{
		channel->setInviteOnly(true);
		for (size_t i = 0; i < users.size(); i++)
		{
			sendResponse(users[i], RPL_MODE(client->getNickName(),client->getUserName(), channelName, modeChar));
		}
	}
	if (modeChar.size() >= 2 && modeChar[0] == '-' && modeChar[1] == 'i')
	{
		channel->setInviteOnly(false);
		for (size_t i = 0; i < users.size(); i++)
		{
			sendResponse(users[i], RPL_MODE(client->getNickName(),client->getUserName(), channelName, modeChar));
		}
	}
}
