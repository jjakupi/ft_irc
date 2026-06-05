NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = Server/main.cpp \
	Server/Server.cpp \
	Server/Init_Socket.cpp \
	Server/Run.cpp \
	Commands/CmdsUtils.cpp \
	Server/ServerUtils.cpp \
	Commands/Auth.cpp \
	Commands/Kick.cpp \
	Commands/Join.cpp \
	Commands/Quit.cpp \
	Client/Client.cpp \
	Client/ClientUtils.cpp \
	Channel/Channel.cpp \
	Commands/PriveMsg.cpp \
	Commands/Invite.cpp \
	Commands/Topic.cpp \
	Commands/Mode.cpp

OBJ_DIR = obj
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
