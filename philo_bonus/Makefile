NAME		:= philo_bonus

SRC_DIR		:= src
OBJ_DIR		:= obj
LIB_DIR		:= lib

# TODO wildcards
# SRC			:= main.c
SRC			:= $(wildcard *.c */*.c)

OBJ			:= $(SRC:%.c=$(OBJ_DIR)/%.o)

# TODO correct flags
CC			:= cc
CPPFLAGS	:= -I . -MMD -MP
CFLAGS		:= -pthread -Wall -Werror -O1#-Wextra
LDFLAGS		:= -pthread
LDLIBS		:=

.PHONY:	all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

-include $(OBJ:.o=.d)
