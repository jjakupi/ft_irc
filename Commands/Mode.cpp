#include "../Server/Server.hpp"

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
		sendResponse(fd, ERR_NEEDMOREPARAMS(cmd));
		return ;
	}

	std::string channelName = args[1];

	if (!isValidChannelName(channelName))
	{
		sendResponse(fd, ERR_BADCHANMASK(client->getNickName(), channelName));
		return ;
	}

	Channel *channel = getChannel(channelName);
	if (!channel)
	{
		sendResponse(fd, ERR_NOSUCHCHANNEL(client->getNickName(), channelName));
		return ;
	}

	if (!channel->hasUser(fd))
	{
		sendResponse(fd, ERR_NOTONCHANNEL(client->getNickName(), channelName));
		return ;
	}

	if (args.size() == 2)
	{
		std::string modes = "+";
		std::string params = "";

		if (channel->isInvitedOnly())
			modes += "i";
		if (channel->isTopicRestricted())
			modes += "t";
		if (channel->hasKey())
			modes += "k";
		if (channel->hasLimit())
		{
			modes += "l";

			std::stringstream ss;
			ss << channel->getLimit();
			params += " " + ss.str();
		}
		sendResponse(fd, RPL_CHANNELMODEIS(client->getNickName(), channelName, modes, params));
		return ;
	}

	if (!channel->isOperator(fd))
	{
		sendResponse(fd, ERR_CHANOPRIVSNEEDED(client->getNickName(), channelName));
		return ;
	}

	std::string modeChar = args[2];

	if (!isValidModeCommand(modeChar))
	{
		sendResponse(fd, ERR_UNKNOWNMODE(client->getNickName(), modeChar));
		return ;
	}

	std::vector<int> users = channel->getUsers();
	if (modeChar.size() >= 2 && modeChar[0] == '+' && modeChar[1] == 'i')
	{
		channel->setInviteOnly(true);
		for (size_t i = 0; i < users.size(); i++)
		{
			sendResponse(users[i], RPL_MODE(client->getNickName(),client->getUserName(), channelName, modeChar));
		}
		return ;
	}
	if (modeChar.size() >= 2 && modeChar[0] == '-' && modeChar[1] == 'i')
	{
		channel->setInviteOnly(false);
		for (size_t i = 0; i < users.size(); i++)
		{
			sendResponse(users[i], RPL_MODE(client->getNickName(),client->getUserName(), channelName, modeChar));
		}
		return ;
	}
	if (modeChar.size() >= 2 && modeChar[0] == '+' && modeChar[1] == 't')
	{
		channel->setTopicRestricted(true);
		for (size_t i = 0; i < users.size(); i++)
			sendResponse(users[i], RPL_MODE(client->getNickName(),
				client->getUserName(), channelName, "+t"));
		return ;
	}

	if (modeChar.size() >= 2 && modeChar[0] == '-' && modeChar[1] == 't')
	{
		channel->setTopicRestricted(false);
		for (size_t i = 0; i < users.size(); i++)
			sendResponse(users[i], RPL_MODE(client->getNickName(),
				client->getUserName(), channelName, "-t"));
		return ;
	}
	if(modeChar.size() >= 2 && modeChar[0] == '+' && modeChar[1] == 'l')
	{
		if (args.size() < 4)
		{
			sendResponse(fd, ERR_NEEDMOREPARAMS(cmd));
			return ;
		}
		std::stringstream ss(args[3]);
		int limit;
		if (!(ss >> limit) || limit <= 0)
		{
			sendResponse(fd, ERR_INVALIDMODEPARAM(client->getNickName(), channelName, "+l", args[3]));
			return ;
		}
		channel->setHasLimit(true);
		channel->setLimit(limit);
		for (size_t i = 0; i < users.size(); i++)
		{
			sendResponse(users[i], RPL_MODE_PARAM(client->getNickName(), client->getUserName(), channelName, "+l", args[3]));
		}
		return ;
	}
	if(modeChar.size() >= 2 && modeChar[0] == '-' && modeChar[1] == 'l')
	{
		channel->setHasLimit(false);
		channel->setLimit(0);
		for (size_t i = 0; i < users.size(); i++)
		{
			sendResponse(users[i], RPL_MODE(client->getNickName(),client->getUserName(), channelName, modeChar));
		}
		return ;
	}
	if(modeChar.size() >= 2 && modeChar[0] == '+' && modeChar[1] == 'o')
	{
		if (args.size() < 4)
		{
			sendResponse(fd, ERR_NEEDMOREPARAMS(cmd));
			return ;
		}
		std::string targetNick = args[3];
		if (!isValidNickName(targetNick))
		{
			sendResponse(fd, ERR_ERRONEUSNICK(targetNick));
			return ;
		}
		Client *targetClient = getClientNick(targetNick);
		if (!targetClient)
		{
			sendResponse(fd, ERR_NOSUCHNICK(client->getNickName(), targetNick));
			return ;
		}
		if (!channel->hasUser(targetClient->getFd()))
		{
			sendResponse(fd, ERR_USERNOTINCHANNEL(targetNick, channelName));
			return ;
		}
		if(channel->isOperator(targetClient->getFd()))
		{
			return;
		}
		channel->addOperator(targetClient->getFd());
		for (size_t i = 0; i < users.size(); i++)
		{
			sendResponse(users[i], RPL_MODE_PARAM(client->getNickName(), client->getUserName(), channelName, "+o", args[3]));
		}
		return ;
	}
	if (modeChar.size() >= 2 && modeChar[0] == '-' && modeChar[1] == 'o')
	{
		if (args.size() < 4)
		{
			sendResponse(fd, ERR_NEEDMOREPARAMS(cmd));
			return ;
		}
		std::string targetNick = args[3];
		if (!isValidNickName(targetNick))
		{
			sendResponse(fd, ERR_ERRONEUSNICK(targetNick));
			return ;
		}
		Client *targetClient = getClientNick(targetNick);
		if (!targetClient)
		{
			sendResponse(fd, ERR_NOSUCHNICK(client->getNickName(), targetNick));
			return ;
		}
		if (!channel->hasUser(targetClient->getFd()))
		{
			sendResponse(fd, ERR_USERNOTINCHANNEL(targetNick, channelName));
			return ;
		}
		if (!channel->isOperator(targetClient->getFd()))
			return ;

		channel->removeOperator(targetClient->getFd());

		for (size_t i = 0; i < users.size(); i++)
		{
			sendResponse(users[i], RPL_MODE_PARAM(client->getNickName(),
				client->getUserName(), channelName, "-o", args[3]));
		}
		return ;
	}
	if(!channel->isOperator(fd))
	{
		sendResponse(fd, ERR_CHANOPRIVSNEEDED(client->getNickName(), channelName));
		return ;
	}
	if (modeChar.size() >= 2 && modeChar[0] == '+' && modeChar[1] == 'k')
	{
		if (args.size() < 4)
		{
			sendResponse(fd, ERR_NEEDMOREPARAMS(cmd));
			return ;
		}

		if (channel->hasKey())
		{
			sendResponse(fd, ERR_KEYSET(client->getNickName(), channelName));
			return ;
		}

		std::string key = cleanKey(args[3]);

		if (!isValidChannelKey(key))
		{
			sendResponse(fd, ERR_INVALIDMODEPARAM(client->getNickName(),
				channelName, "+k", key));
			return ;
		}
		channel->setKey(key);
		for (size_t i = 0; i < users.size(); i++)
		{
			sendResponse(users[i], RPL_MODE_PARAM(client->getNickName(),
				client->getUserName(), channelName, "+k", key));
		}
		return ;
	}
	if (modeChar.size() >= 2 && modeChar[0] == '-' && modeChar[1] == 'k')
	{
		if (channel->hasKey())
		{
			channel->removeKey();
			for (size_t i = 0; i < users.size(); i++)
			{
				sendResponse(users[i], RPL_MODE(client->getNickName(),
					client->getUserName(), channelName, "-k"));
			}
		}
		return ;
	}
}
