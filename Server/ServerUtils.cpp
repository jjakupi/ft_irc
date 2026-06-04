#include "Server.hpp"
#include "../Client/Client.hpp"

void	Server::acceptClient()
{
	{
		int clientFd = accept(_serverFd, NULL, NULL);
		if (clientFd == -1)
		{
			perror("accept");
				return;
		}
		std::cout << "New client connected, fd = "<< clientFd << std::endl;
		pollfd clientPoll;
		clientPoll.fd = clientFd;
		clientPoll.events = POLLIN;
		clientPoll.revents = 0;
		_pollFds.push_back(clientPoll);
		_clients.push_back(Client(clientFd));
	}
}

void	Server::receiveFromClient(int fd)
{
	std::vector<char> buf(5000);
	int bytes = recv(fd, buf.data(), buf.size(), 0);

	if (bytes > 0)
	{
		std::string msg(buf.data(), bytes);
		Client *client = getClient(fd);
		//std::cout << "MSG=[" << msg << "]" << std::endl;
		client->appendBuffer(msg);
		//std::cout << "BUFFER=[" << client->getBuffer() << "]" << std::endl;
		while (client->hasCompleteCommand())
		{
			std::string command = client->popCommand();
			//std::cout << "RAW=[" << command << "]" << std::endl;
			command = client->cleanCommand(command);
			std::cout << "CLEAN COMMAND=[" << command << "]" << std::endl;
			if (!command.empty())
			{
				this->getCommand(command, fd);
			}
		}
	}
	else if (bytes == 0)
	{
		std::cout << "Client " << fd << " disconnected" << std::endl;
		removeClient(fd);
	}
	else
	{
		perror("recv");
		removeClient(fd);
	}
}

void	Server::removeClient(int fd)
{
	close(fd);

	for (size_t i = 0; i < _pollFds.size(); i++)
	{
		if (_pollFds[i].fd == fd)
		{
			_pollFds.erase(_pollFds.begin() + i);
			break;
		}
	}

	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getFd() == fd)
		{
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
}
// end ServerUtils
void Server::getCommand(std::string command, int fd)
{
	std::string cmdName = command.substr(0, command.find(' '));
	for(size_t i = 0; i < cmdName.size(); i++)
		cmdName[i] = std::toupper(cmdName[i]);
	if (cmdName == "PASS")
		this->pass(command, fd);
	else if(cmdName == "NICK")
		this->nick(command, fd);
	else if(cmdName == "USER")
		this->user(command, fd);
	else if(cmdName == "JOIN")
		this->join(command, fd);
	else if(cmdName == "PRIVMSG")
		this->privmsg(command, fd);
	else if(cmdName == "QUIT")
		this->quit(command, fd);
	else if(cmdName == "KICK")
		this->kick(command, fd);
	else if(cmdName == "INVITE")
		this->invite(command, fd);
	else if(cmdName == "TOPIC")
		this->topic(command, fd);
	else if(cmdName == "MODE")
		this->mode(command, fd);
	else
		sendResponse(fd, ERR_UNKNOWNCOMMAND(cmdName));
}

Channel	*Server::getChannel(std::string channelName)
{
	std::string searchedName = normalizeChannelName(channelName);

	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (normalizeChannelName(_channels[i].getChannelName()) == searchedName)
			return (&_channels[i]);
	}
	return (NULL);
}

void Server::addChannel(std::string channelName)
{
	Channel newChannel(channelName);
	_channels.push_back(newChannel);
}
