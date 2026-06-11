#include "../Server/Server.hpp"

void Server::quit(std::string cmd, int fd)
{
	Client *client = getClient(fd);
	if (!client)
		return ;

	std::cout << RED << "Client " << fd << " disconnected" << WHI << std::endl;

	std::string nick = client->getNickName();
	std::string user = client->getUserName();

	std::string reason = "Client disconnected";
	size_t pos = cmd.find(':');
	if (pos != std::string::npos)
		reason = cmd.substr(pos + 1);

	for (size_t i = 0; i < _channels.size(); )
	{
		if (_channels[i].hasUser(fd))
		{
			std::vector<int> users = _channels[i].getUsers();

			for (size_t j = 0; j < users.size(); j++)
			{
				if (users[j] != fd)
					sendResponse(users[j], RPL_QUIT(nick, user, reason));
			}

			bool wasOperator = _channels[i].isOperator(fd);

			_channels[i].removeUser(fd);
			_channels[i].removeOperator(fd);

			if (_channels[i].getUsers().empty())
			{
				_channels.erase(_channels.begin() + i);
				continue ;
			}

			if (wasOperator && !_channels[i].hasOperators())
				_channels[i].addOperator(_channels[i].getUsers()[0]);
		}
		i++;
	}

	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getFd() == fd)
		{
			_clients.erase(_clients.begin() + i);
			break ;
		}
	}

	for (size_t i = 0; i < _pollFds.size(); i++)
	{
		if (_pollFds[i].fd == fd)
		{
			_pollFds.erase(_pollFds.begin() + i);
			break ;
		}
	}

	close(fd);
}
