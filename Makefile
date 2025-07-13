# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/13 15:47:17 by eduaserr          #+#    #+#              #
#    Updated: 2025/07/13 03:39:29 by eduaserr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### COMPILATION ###
NAME	= philo
CC		= clang
CFLAGS	= -Wall -Wextra -Werror -g -pthread

MAKE	= make --no-print-directory
RM		= rm -f

### SRCS ###
LIB		= lib/
INIT	= src/init/
UTLS	= src/utils/

SRC		= main.c
SRCS	= $(LIB)ft_atoi.c $(LIB)ft_isdigit.c $(LIB)ft_strcmp.c\
		$(INIT)init_parser.c $(INIT)init_table.c\
		$(UTLS)utils_error.c $(UTLS)utils.c

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
