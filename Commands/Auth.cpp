#include "../Server/Server.hpp"
#include "Replies.hpp"
#include "../Client/Client.hpp"

// PASS

void Server::pass(std::string cmd, int fd) {
	if (cmd.size() <= 5) {
		sendResponse(fd, ERR_NEEDMOREPARAMS(cmd));
		return ;
	}
	Client *client = getClient(fd);
	if (!client)
		return ;
	std::string mdp = cmd.substr(5);
	if (mdp[0] == ':')
		mdp = mdp.substr(1);
	if (!client->getRegistered()) {
		if (mdp == _password)
			client->setRegistered(true);
		else {
			sendResponse(fd, ERR_WRONGPASSWORD);
			return ;
		}
	}
	else {
		sendResponse(fd, ERR_ALREADYREGISTERED(client->getNickName()));
		return ;
	}
}

// NICK

void Server::nick(std::string cmd, int fd) {
	if (cmd.size() <= 5) {
		sendResponse(fd, ERR_NEEDMOREPARAMS(cmd));
		return ;
	}
	Client *client = getClient(fd);
	if (!client)
		return ;
	std::string nickname = cmd.substr(5);
	if (nickname[0] == ':')
		nickname = nickname.substr(1);
	if (!client->getRegistered()) {
		sendResponse(fd, ERR_NOTREGISTERED(nickname));
		return ;
	}
	if (!isValidNickName(nickname)) {
		sendResponse(fd, ERR_ERRONEUSNICK(nickname));
		return ;
	}
	if (nickNameAlreadyUsed(nickname)) {
		sendResponse(fd, ERR_NICKINUSE(nickname));
		return ;
	}
	client->setNickName(nickname);
	tryRegister(client, fd);
}

// USER

 void    Server::user(std::string cmd, int fd)
{
    Client *client = getClient(fd);
    if (!client)
        return ;
    std::vector<std::string> args = splitCommand(cmd);
    if (args.size() < 5) {
        sendResponse(fd, ERR_NEEDMOREPARAMS(args[0]));
        return ;
    }
    std::string nickname;
    if (client->getNickName().empty())
        nickname = "*";
    else
        nickname = client->getNickName();
    if (!client->getRegistered()) {
        sendResponse(fd, ERR_NOTREGISTERED(nickname));
        return ;
    }
    if (!client->getUserName().empty()) {
        sendResponse(fd, ERR_ALREADYREGISTERED(nickname));
        return ;
    }
    if (!isValidUserName(args[1])) {
        sendResponse(fd, ERR_INVALIDUSERNAME(args[1]));
        return ;
    }
    client->setUserName(args[1]);
    tryRegister(client, fd);
}
