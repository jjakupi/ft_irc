#include "Client.hpp"

void	Client::appendBuffer(const std::string &data)
{
	_buffer +=  data;
}

bool	Client::hasCompleteCommand() const
{
	return (_buffer.find('\n') != std::string::npos);
}

std::string	Client::popCommand()
{
	std::string command;
	std::string::size_type pos = _buffer.find("\n");

	if (pos == std::string::npos);
	command = _buffer.substr(0, pos);
	_buffer.erase(0, pos + 1);

	if (!command.empty() && command[command.size() - 1] == '\r')
		command.pop_back();
	return command;
}

std::string	Client::cleanspaces(std::string command)
{
	std::string spaces = " \t";
	std::string::size_type start = command.find_first_not_of(spaces);

	if (start == std::string::npos)
		return ("");

	std::string::size_type end = command.find_last_not_of(spaces);

	command = (command.substr(start, end - start + 1));
	std::string result;
	bool wasSpace = false;

	for (size_t i = 0; i < command.size(); i++)
	{
		if (command[i] == ' ' || command[i] == '\t')
		{
			if (wasSpace == false)
			{
				result += ' ';
				wasSpace = true;
			}
		}
		else
		{
			result += command[i];
			wasSpace = false;
		}
	}
	return result;
}

std::string	Client::cleanCommand(std::string command)
{
	std::string::size_type pos = command.find(':');
	if(pos == std::string::npos)
	{
		cleanspaces(command);
		return command;
	}
	else
	{
		std::string before = command.substr(0, pos);
		std::string after = command.substr(pos);
		before = cleanspaces(before);
		return before + " " + after;
	}
}
