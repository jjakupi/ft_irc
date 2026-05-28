NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = Server/main.cpp \
	Server/Server.cpp \
	Server/Init_Socket.cpp \
	Server/Run.cpp \
	Commands/helpers.cpp \
	Server/ServerUtils.cpp \
	Commands/Auth.cpp \
	Client/Client.cpp \
	Client/ClientUtils.cpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
