#include "../Server/Server.hpp"

void Server::kick(std::string cmd, int fd) {
	Client *client = getClient(fd);
	if (!client)
		return ;

	if (!client->getLoggedIn()) {
		sendResponse(fd, ERR_NOTREGISTERED(client->getNickName()));
		return ;
	}

	std::vector<std::string> args = splitCommand(cmd);
	if (args.size() < 3) {
		sendResponse(fd, ERR_NEEDMOREPARAMS(args[0]));
		return ;
	}
	std::string reason = "No reason given";
	if (args.size() >= 4)
		reason = args[3];
	std::string channelName = args[1];
	if (!isValidChannelName(channelName))
	{
		sendResponse(fd, ERR_BADCHANMASK(client->getNickName(), channelName));
		return ;
	}
	Channel *channel = getChannel(channelName);
	if (!channel) {
		sendResponse(fd, ERR_NOSUCHCHANNEL(client->getNickName(), args[1]));
		return ;
	}
	if (!channel->hasUser(fd))
	{
		sendResponse(fd, ERR_NOTONCHANNEL(client->getNickName(), channelName));
		return ;
	}
	if (!channel->isOperator(fd)) {
		sendResponse(fd, ERR_CHANOPRIVSNEEDED(client->getNickName(), args[1]));
		return ;
	}

	Client *target = getClientNick(args[2]);
	if (!target || !channel->hasUser(target->getFd())) {
		sendResponse(fd, ERR_USERNOTINCHANNEL(args[2], args[1]));
		return ;
	}

	std::vector<int> users = channel->getUsers();
	for (size_t j = 0; j < users.size(); j++)
		sendResponse(users[j], RPL_KICK(client->getNickName(), client->getUserName(), args[1], args[2], reason));

	bool wasOperator = channel->isOperator(target->getFd());
	channel->removeUser(target->getFd());
	if (channel->getUsers().empty()) {
		for (size_t i = 0; i < _channels.size(); i++) {
			if (&_channels[i] == channel) {
				_channels.erase(_channels.begin() + i);
				break;
			}
		}
	}
	else if (wasOperator)
		channel->addOperator(channel->getUsers()[0]);
}

