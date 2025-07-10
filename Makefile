# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/13 15:47:17 by eduaserr          #+#    #+#              #
#    Updated: 2025/07/10 21:02:37 by eduaserr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### COMPILATION ###
NAME	= philo
CC		= clang
CFLAGS	= -Wall -Wextra -Werror -g

MAKE	= make --no-print-directory
RM		= rm -f

### SRCS ###
SRC		= main.c
SRCS	= src/utils/utils.c

### OBJS ###
OBJS	= $(SRC:.c=.o) $(SRCS:.c=.o)

### RULES ###
all : $(NAME)

$(NAME): $(OBJS)
	@echo "loading philo..."
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "philo compiled successfully"

%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "clearing philo...🧹"
	@$(RM) $(OBJS) main.o

fclean: clean
	@$(RM) $(NAME) main
	@echo "clearing philo executable"

re: fclean all

.PHONY: all clean fclean re
