# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aylee <aylee@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/12 00:00:00 by aylee             #+#    #+#              #
#    Updated: 2026/02/23 14:54:49 by aylee            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline

SRCS = main.c \
       built_in.c \
       env.c \
       utils.c \
       main_init.c \
       exec.c \
	   parse.c

OBJS = $(SRCS:.c=.o)
LIBFT = ./libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -s -C ./libft bonus
	@echo " [ ok ] | all libft is readly! "

clean:
	rm -f $(OBJS)
	@make -s -C ./libft fclean
	@echo " [ ok ] | all libft is clean! "

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
