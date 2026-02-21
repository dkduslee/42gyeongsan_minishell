NAME = minishell 
CC = cc -Wall -Werror -Wextra
READLINE = -lreadline
SRC = built_in.c env.c error_print.c minishell.c
OBJ = $(SRC:.c=.o)
LIBFT = ./libft/libft.a

all : $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(READLINE) $(OBJ) $(LIBFT) -o $@

$(LIBFT):
	make -C ./libft bonus

%.o : %.c minishell.h
	$(CC) -c $< -o $@
	
clean:
	@make -C ./libft fclean
	rm -f $(OBJ)
	
fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
	
