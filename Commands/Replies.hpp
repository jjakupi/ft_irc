#ifndef REPLIES_HPP
#define REPLIES_HPP

#define ERR_NEEDMOREPARAMS(cmd) (":server 461 * " + cmd + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTERED(nick) (":server 462 " + nick + " :You may not reregister\r\n")
#define ERR_WRONGPASSWORD (":server 464 * :Password incorrect\r\n")
#define ERR_NOTREGISTERED(nick) (":server 451 " + nick + " :You have not registered\r\n")
#define ERR_ERRONEUSNICK(nick) (std::string(":server 432 * ") + nick + " :Erroneous nickname\r\n")
#define ERR_NICKINUSE(nick) (std::string(":server 433 * ") + nick + " :Nickname is already in use\r\n")
#define RPL_WELCOME(nick, user) \
	(std::string(":server 001 ") + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "\r\n")
#endif
