NAME = ircserv
CC = c++
CFLAG = -Wall -Wextra -Werror -std=c++98

SRCS =	main.cpp \
		signal.cpp \
		Client.cpp \
		Server.cpp \
		utils.cpp 

INCLUDE =	lib.hpp \
			Client.hpp \
			Server.hpp 
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