#include "Server.hpp"

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	int port = std::atoi(argv[1]);
	std::string password = argv[2];

	Server server(port, password);

	if (server.initSocket() == -1)
		return (1);

	if (server.run() == -1)
		return (1);

	return (0);
}
