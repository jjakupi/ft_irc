#include "../Server/Server.hpp"
#include "Replies.hpp"
#include "../Client/Client.hpp"

// PASS

void	Server::pass(std::string cmd, int fd)
{
	Client						*client;
	std::vector<std::string>	args;
	std::string					mdp;

	client = getClient(fd);
	std::cout << "DEBUG PASS: registered="
	<< client->getRegistered()
	<< " loggedIn=" << client->getLoggedIn()
	<< " nick=[" << client->getNickName() << "]"
	<< " user=[" << client->getUserName() << "]"
	<< std::endl;
	if (!client)
		return ;

	args = splitCommand(cmd);
	if (args.size() < 2)
	{
		sendResponse(fd, ERR_NEEDMOREPARAMS(cmd));
		return ;
	}

	if (client->getLoggedIn())
	{
		sendResponse(fd, ERR_ALREADYREGISTERED(client->getNickName()));
		return ;
	}

	if (client->getRegistered())
	{
		sendResponse(fd, ERR_ALREADYREGISTERED(client->getNickName()));
		return ;
	}

	mdp = args[1];
	if (!mdp.empty() && mdp[0] == ':')
		mdp = mdp.substr(1);

	if (mdp != _password)
	{
		sendResponse(fd, ERR_WRONGPASSWORD);
		return ;
	}

	client->setRegistered(true);
	tryRegister(client, fd);
}

// NICK

void	Server::nick(std::string cmd, int fd)
{
	Client						*client;
	std::vector<std::string>	args;
	std::string					nickname;

	client = getClient(fd);
	if (!client)
		return ;

	args = splitCommand(cmd);
	if (args.size() < 2)
	{
		sendResponse(fd, ERR_NEEDMOREPARAMS(cmd));
		return ;
	}

	nickname = args[1];

	if (!isValidNickName(nickname))
	{
		sendResponse(fd, ERR_ERRONEUSNICK(nickname));
		return ;
	}

	if (nickNameAlreadyUsed(nickname))
	{
		sendResponse(fd, ERR_NICKINUSE(nickname));
		return ;
	}

	client->setNickName(nickname);
	tryRegister(client, fd);
}

void	Server::user(std::string cmd, int fd)
{
	Client *client = getClient(fd);
	if (!client)
		return ;

	std::vector<std::string> args = splitCommand(cmd);

	if (args.size() < 5)
	{
		sendResponse(fd, ERR_NEEDMOREPARAMS(cmd));
		return ;
	}

	if (client->getLoggedIn())
	{
		sendResponse(fd, ERR_ALREADYREGISTERED(client->getUserName()));
		return ;
	}

	client->setUserName(args[1]);
	tryRegister(client, fd);
}
