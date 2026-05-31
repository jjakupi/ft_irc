#include "../Server/Server.hpp"
#include "Replies.hpp"
#include "../Client/Client.hpp"
#include "../Channel/Channel.hpp"

void Server::quit(std::string cmd, int fd) {
	Client *client = getClient(fd);
	if (!client)
		return;
	std::string reason = "Client disconnected";
	size_t pos = cmd.find(':');
	if (pos != std::string::npos)
		reason = cmd.substr(pos + 1);

	for (size_t i = 0; i < _channels.size(); i++) {
		if (_channels[i].hasUser(fd)) {
			std::vector<int> users = _channels[i].getUsers();
			for (size_t j = 0; j < users.size(); j++) {
				if (users[j] != fd)
					sendResponse(users[j], RPL_QUIT(client->getNickName(), client->getUserName(), reason));
			}
			bool wasOperator = _channels[i].isOperator(fd);
			_channels[i].removeUser(fd);
			if (_channels[i].getUsers().empty()) {
				_channels.erase(_channels.begin() + i);
				i--;
				continue;
			}
			else {
				if (wasOperator)
					_channels[i].addOperator(_channels[i].getUsers()[0]);
			}
		}
	}

	for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i].getFd() == fd) {
			_clients.erase(_clients.begin() + i);
			break;
		}
	}

	for (size_t i = 0; i < _pollFds.size(); i++) {
		if (_pollFds[i].fd == fd) {
			_pollFds.erase(_pollFds.begin() + i);
			break;
		}
	}

	close(fd);
}

void Channel::removeUser(int fd) {
	std::vector<int>::iterator it = std::find(_users.begin(), _users.end(), fd);
	if (it != _users.end())
		_users.erase(it);

	std::vector<int>::iterator it2 = std::find(_operators.begin(), _operators.end(), fd);
	if (it2 != _operators.end())
		_operators.erase(it2);
}
