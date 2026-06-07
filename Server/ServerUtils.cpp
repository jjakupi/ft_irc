#include "Server.hpp"

void	Server::acceptClient()
{
		int clientFd = accept(_serverFd, NULL, NULL);
		if (clientFd == -1)
		{
			perror("accept");
				return;
		}
		if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
		{
			perror("fcntl");
			close(clientFd);
			return ;
		}
		std::cout << "New client connected, fd = "<< clientFd << std::endl;
		pollfd clientPoll;
		clientPoll.fd = clientFd;
		clientPoll.events = POLLIN;
		clientPoll.revents = 0;
		_pollFds.push_back(clientPoll);
		_clients.push_back(Client(clientFd));
}

void	Server::receiveFromClient(int fd)
{
	std::vector<char> buf(5000);
	int bytes = recv(fd, buf.data(), buf.size(), 0);

	if (bytes > 0)
	{
		std::string msg(buf.data(), bytes);
		Client *client = getClient(fd);
		if (!client)
			return ;
		client->appendBuffer(msg);
		while (client->hasCompleteCommand())
		{
			std::string command = client->popCommand();
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
		quit("QUIT :Client disconnected", fd);
	}
	else
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return ;
		perror("recv");
		quit("QUIT :Connection error", fd);
	}
}

// end ServerUtils
void Server::getCommand(std::string command, int fd)
{
	std::string cmdName = command.substr(0, command.find(' '));
	std::string cmdOriginal = cmdName;
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
		sendResponse(fd, ERR_UNKNOWNCOMMAND(cmdOriginal));
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
