NAME		:= philo

SRC_DIR		:= src
OBJ_DIR		:= obj
LIB_DIR		:= lib

SRC			:= bootstrap.c \
               bootstrap2.c \
               main.c \
               philo_actions.c \
               routines.c \
               run.c \
               utils.c

OBJ			:= $(SRC:%.c=$(OBJ_DIR)/%.o)

CC			:= cc
CPPFLAGS	:= -I . -MMD -MP

CFLAGS		:= -Wall -Werror -Wextra -O1#-g -fsanitize=thread
LDFLAGS		:= -pthread#-fsanitize=thread
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
