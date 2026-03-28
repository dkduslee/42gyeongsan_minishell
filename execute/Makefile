# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aylee <aylee@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/12 00:00:00 by aylee             #+#    #+#              #
#    Updated: 2026/03/02 19:52:53 by aylee            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -I.. -I/opt/homebrew/opt/readline/include
AR		= ar rcs

SRCS	= built_in.c cmd_path.c command.c env.c exec.c exec2.c exit.c export.c \
		heredoc.c image.c main_init.c main.c make_node.c pipe.c utils.c
OBJS	= $(SRCS:.c=.o)
LIB		= libexec.a

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) $(LIB) $(OBJS)

%.o: %.c ../minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(LIB)

re: fclean all

.PHONY: all clean fclean re
