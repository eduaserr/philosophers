# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/12 14:05:06 by eduaserr          #+#    #+#              #
#    Updated: 2025/03/13 18:18:05 by eduaserr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### COMPILATION ###
NAME	= libft.a
CC	= clang
CFLAGS	= -Wall -Wextra -Werror
RM	= rm -f
AR	= ar rcs

### SRCS ###
SRCS	= func/ft_isspace.c func/ft_isalnum.c func/ft_isalpha.c func/ft_isascii.c func/ft_isdigit.c func/ft_isprint.c func/ft_toupper.c func/ft_tolower.c func/ft_putchar_fd.c func/ft_putnbr_fd.c func/ft_putstr_fd.c func/ft_putendl_fd.c\
		func/ft_memset.c func/ft_bzero.c func/ft_calloc.c func/ft_memchr.c func/ft_memcpy.c func/ft_memmove.c func/ft_memcmp.c\
		func/ft_strchr.c func/ft_strrchr.c func/ft_strncmp.c func/ft_strnstr.c func/ft_strdup.c func/ft_substr.c func/ft_strjoin.c func/ft_strtrim.c func/ft_strmapi.c func/ft_striteri.c\
		func/ft_atoi.c func/ft_strlcat.c func/ft_strlcpy.c func/ft_strlen.c func/ft_itoa.c func/ft_split.c\
		func/ft_printlines.c func/ft_printmap.c func/ft_arrdup.c func/ft_freemap.c

BONUS	= func/ft_lstnew_bonus.c func/ft_lstadd_front_bonus.c func/ft_lstsize_bonus.c func/ft_lstlast_bonus.c func/ft_lstadd_back_bonus.c\
		func/ft_lstdelone_bonus.c func/ft_lstclear_bonus.c func/ft_lstiter_bonus.c func/ft_lstmap_bonus.c

PF_SRCS	= ft_printf/ft_printf.c ft_printf/ft_funlibft_pf.c

GNL_SRCS = Get_Next_line/get_next_line_bonus.c Get_Next_line/get_next_line_utils_bonus.c

### OBJS ###
PF_OBJS = $(PF_SRCS:.c=.o)
GNL_OBJS = $(GNL_SRCS:.c=.o)
OBJS	= $(SRCS:.c=.o) $(PF_OBJS) $(GNL_OBJS)
BONUS_OB = $(BONUS:.c=.o)

### RULES ###
all : $(NAME)

$(NAME): $(OBJS)
	@echo "loading libft..."
	@$(AR) $(NAME) $(OBJS)
	@echo "libft compiled successfully"

bonus : $(OBJS) $(BONUS_OB)
	@(AR) $(NAME) $(OBJS) $(BONUS_OB)

printf: $(PF_OBJS)
	@$(AR) $(NAME) $(PF_OBJS)
	@echo "printf compiled successfully"

gnl: $(GNL_OBJS)
	@$(AR) $(NAME) $(GNL_OBJS)
	@echo "GNL compiled successfully"

%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS) $(BONUS_OB)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

rebonus : fclean bonus

.PHONY: all clean fclean re bonus rebonus printf gnl
