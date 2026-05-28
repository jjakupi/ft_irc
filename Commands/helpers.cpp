#include "../Server/Server.hpp"
#include "Replies.hpp"
#include "../Client/Client.hpp"

std::vector<std::string>	Server::splitCommand(std::string cmd)
{
	std::vector<std::string>	args;
	std::istringstream		iss(cmd);
	std::string				word;

	while (iss >> word)
		args.push_back(word);
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
