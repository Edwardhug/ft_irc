NAME = ircserv
CC = c++
CFLAG = -Wall -Wextra -Werror -std=c++98 -g3

SRCS =	main.cpp \
		signal.cpp \
		Client.cpp \
		Server.cpp \
		utils.cpp \
		Channel.cpp \
		Modes.cpp \
		Invite.cpp \
		Topic.cpp \
		Join.cpp \
		Kick.cpp \
		PrivateMsg.cpp \
		Password.cpp \
		Find.cpp \
		Error.cpp \
		Name.cpp \
#				Bot.cpp \

INCLUDE =	lib.hpp \
			Client.hpp \
			Server.hpp \
			Channel.hpp \
			Error.hpp \
#			Bot.hpp \

OBJS = $(SRCS:%.cpp=$(PATH_OBJS)%.o)

PATH_SRCS = srcs/
PATH_INCLUDE = includes/
PATH_OBJS = objs/

all: $(NAME)

$(NAME): $(PATH_OBJS) $(OBJS) $(addprefix $(PATH_INCLUDE), $(INCLUDE))
	$(CC) $(CFLAG) $(OBJS) -o $(NAME)

$(OBJS): $(PATH_OBJS)%.o: $(PATH_SRCS)%.cpp $(addprefix $(PATH_INCLUDE), $(INCLUDE))
	$(CC) $(CFLAG) -I$(PATH_INCLUDE) -c $< -o $@

$(PATH_OBJS):
	mkdir -p $(PATH_OBJS)

clean:
	rm -rf $(OBJS) $(PATH_OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

FORCE:

.PHONY: all re bonus clean fclean