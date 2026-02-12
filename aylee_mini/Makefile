NAME = minishell
DIR = 
CC = cc -Wall -Werror -Wextra -lreadline
SRC = built_in.c env.c minishell.c
OBJ = $(SRC:.c=.o)
LIBFT = ./libft/libft.a

all : $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(OBJ) $(LIBFT) -o $@
	
%.o : %.c
	$(CC) -c $< -o $@
	
$(LIBFT):
	make -C ./libft bonus
	
clean:
	@make -C ./libft fclean
	rm -f $(OBJ)
	
fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
	
