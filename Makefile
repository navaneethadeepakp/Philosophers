# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: naperisw <naperisw@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/13 18:32:59 by naperisw          #+#    #+#              #
#    Updated: 2026/01/30 13:12:38 by naperisw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME   	= philo
CC	   	= cc
CFLAGS 	= -Wall -Wextra -Werror -pthread

OBJ_DIR = obj

SRCS 	= main.c infos.c philo_routine.c helpers.c utils.c monitor.c \
		  forks.c start_philos.c
OBJS 	= $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re