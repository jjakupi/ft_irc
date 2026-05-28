#include "../Server/Server.hpp"
#include "Replies.hpp"
#include "../Client/Client.hpp"

std::vector<std::string>	Server::splitCommand(std::string cmd)
{
	std::vector<std::string>	args;
	size_t					i;
	size_t					start;

	i = 0;
	while (i < cmd.size())
	{
		while (i < cmd.size() && cmd[i] == ' ')
			i++;

		if (i >= cmd.size())
			break ;

		if (cmd[i] == ':')
		{
			args.push_back(cmd.substr(i + 1));
			break ;
		}

		start = i;
		while (i < cmd.size() && cmd[i] != ' ')
			i++;

		args.push_back(cmd.substr(start, i - start));
	}
	return (args);
}

void	Server::tryRegister(Client *client, int fd)
{
	if (!client)
		return ;

	if (client->getRegistered()
		&& !client->getNickName().empty()
		&& !client->getUserName().empty()
		&& !client->getLoggedIn())
	{
		client->setLoggedIn(true);
		sendResponse(fd, RPL_WELCOME(client->getNickName(), client->getUserName()));
	}
}
