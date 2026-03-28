CC		= cc
CFLAGS	= -Wall -Wextra -Werror -I.. -I/opt/homebrew/opt/readline/include
AR		= ar rcs

SRCS	= error.c expand.c lexer_utils.c lexer.c lexer_quote.c \
		parse.c parse_argv.c parse_utils.c signals.c
OBJS	= $(SRCS:.c=.o)
LIB		= libparse.a

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) $(LIB) $(OBJS)

%.o: %.c ../minishell.h parse_int.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(LIB)

re: fclean all

.PHONY: all clean fclean re
