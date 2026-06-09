#include "Server.hpp"

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	signal(SIGINT, Server::handleSignal);

	int port = std::atoi(argv[1]);
	std::string password = argv[2];

	if (password.empty() || password[0] == ':')
	{
		std::cerr << "Error: password cannot start with ':'" << std::endl;
		return (1);
	}

	Server server(port, password);

	if (server.initSocket() == -1)
	{
		std::cerr << RED << "Server closed" << std::endl;
		server.cleanup();
		return (1);
	}

	if (server.run() == -1)
	{
		std::cerr << RED << "Server closed" << std::endl;
		server.cleanup();
		return (1);
	}
	std::cerr << RED << "Server closed" << std::endl;
	server.cleanup();
	return (0);
}
