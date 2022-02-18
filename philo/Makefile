NAME		:= philo

SRC_DIR		:= src
OBJ_DIR		:= obj
LIB_DIR		:= lib

SRC_FILES	:= main.c \
               utils.c

SRC			:= $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ			:= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC			:= cc
CPPFLAGS	:= -I include -MMD -MP
CFLAGS		:= -g -pthread -Wall -Werror -O1#-Wextra
LDFLAGS		:= -pthread
LDLIBS		:=

.PHONY:	all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

-include $(OBJ:.o=.d)
