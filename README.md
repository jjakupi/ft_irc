# ft_irc

*This project has been created as part of the 42 curriculum by jjakupi and julrusse.*

---

## Description

**ft_irc** is a fully functional IRC server written in **C++98**, built as part of the 42 school curriculum.

IRC (Internet Relay Chat) is a real-time text communication protocol created in 1988. This project implements an IRC server capable of handling multiple simultaneous clients using a single `poll()` call for all Input/Output operations. Clients can connect using a standard IRC client (e.g. LimeChat or `nc`).

### Features

- Multi-client support via non-blocking I/O and `poll()`
- TCP/IP communication (IPv4)
- Full client authentication (password, nickname, username)
- Channel creation and management
- Operator privileges with dedicated commands
- Compliant with IRC RFC 1459 / RFC 2812

### Implemented Commands

| Command | Description |
|---------|-------------|
| `PASS` | Authenticate with server password |
| `NICK` | Set or change nickname |
| `USER` | Register username and real name |
| `JOIN` | Join or create a channel |
| `PART` | Leave a channel |
| `PRIVMSG` | Send a message to a user or channel |
| `QUIT` | Disconnect from the server |
| `KICK` | Remove a user from a channel (operator only) |
| `INVITE` | Invite a user to a channel (operator only) |
| `TOPIC` | View or set the channel topic (operator only) |
| `MODE` | Set channel modes: `i`, `t`, `k`, `o`, `l` (operator only) |

---

## Instructions

### Requirements

- A C++98 compatible compiler (`c++`)
- macOS or Linux
- An IRC client (e.g. [LimeChat](http://limechat.net/mac/), or `nc`)

### Compilation

```bash
make
```

### Running the server

```bash
./ircserv <port> <password>
```

- `port` — the port number the server will listen on (e.g. `6667`)
- `password` — the connection password required by any IRC client

**Example:**
```bash
./ircserv 6667 mypassword
```

### Connecting with an IRC client

**LimeChat:**
1. File → New Server
2. Fill in: Server = `localhost`, Port = `6667`, Password = `mypassword`
3. Set a nickname and click Connect
4. Type `/join #channel` to join or create a channel

**With `nc` (for testing):**
```bash
nc localhost 6667
PASS mypassword
NICK julia
USER julia 0 * :Julia
JOIN #test
PRIVMSG #test :Hello!
```

### Cleanup

```bash
make fclean
```

---

## Resources

### IRC Protocol References

- [RFC 1459 — Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
- [RFC 2810 — IRC Architecture](https://datatracker.ietf.org/doc/html/rfc2810)
- [RFC 2811 — IRC Channel Management](https://datatracker.ietf.org/doc/html/rfc2811)
- [RFC 2812 — IRC Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/)

### Useful Tools

- [LimeChat IRC client for macOS](http://limechat.net/mac/)

### AI Usage

Claude was used as a learning assistant to start this project, specifically for:

- **Understand the IRC protocol:** clarifying RFC rules for commands such as `PASS`, `NICK`, `USER`, `KICK`, `QUIT`, and `MODE`

- **Help find reliable documentation:** RFC protocol references

- **Git workflow:** guidance on branches, pull requests, and collaborative development practices

- **Write a structured README:** detailing all steps and specific commands

All code was written and understood by us. AI was used as a pedagogical tool, not as a code generator.
