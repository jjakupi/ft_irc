#ifndef REPLIES_HPP
#define REPLIES_HPP

#define ERR_NEEDMOREPARAMS(cmd) (":server 461 * " + cmd + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTERED(nick) (":server 462 " + nick + " :You may not reregister\r\n")
#define ERR_WRONGPASSWORD (":server 464 * :Password incorrect\r\n")
#define ERR_NOTREGISTERED(nick) (":server 451 " + nick + " :You have not registered\r\n")
#define ERR_ERRONEUSNICK(nick) (std::string(":server 432 * ") + nick + " :Erroneous nickname\r\n")
#define ERR_NICKINUSE(nick) (std::string(":server 433 * ") + nick + " :Nickname is already in use\r\n")
#define RPL_WELCOME(nick, user) (std::string(":server 001 ") + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "\r\n")
#define ERR_BADCHANMASK(channel) (std::string(":server 476 ") + channel + " :Bad Channel Mask\r\n")
#define RPL_JOIN(nick, user, channel) (std::string(":") + nick + "!" + user + "@localhost JOIN " + channel + "\r\n")
#define RPL_NAMREPLY(nick, channel, names) (std::string(":server 353 ") + nick + " = " + channel + " :" + names + "\r\n")
#define RPL_ENDOFNAMES(nick, channel) (std::string(":server 366 ") + nick + " " + channel + " :End of /NAMES list\r\n")
#define RPL_WELCOME(nick, user) (std::string(":server 001 ") + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "\r\n")
#define ERR_INVALIDUSERNAME(user) (std::string(":server 468 * ") + user + " :Your username is not valid\r\n")

#endif
