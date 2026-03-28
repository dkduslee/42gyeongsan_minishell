NAME	= minishell
CC		= cc
LDFLAGS	= -lreadline -L/opt/homebrew/opt/readline/lib

LIBFT	= libft/libft.a
LIBPARSE= parse/libparse.a
LIBEXEC	= execute/libexec.a

all: $(NAME)

$(NAME): $(LIBFT) $(LIBPARSE) $(LIBEXEC)
	$(CC) $(LDFLAGS) $(LIBEXEC) $(LIBPARSE) $(LIBFT) -o $(NAME)

$(LIBFT):
	@make -s -C libft bonus
	@echo " [ ok ] | all libft is readly! "

$(LIBPARSE):
	@make -s -C parse
	@echo " [ ok ] | minishell is readly! "

$(LIBEXEC):
	@make -s -C execute

clean:
	@make -s -C parse clean
	@make -s -C execute clean
	@make -s -C libft clean
	@echo " [ ok ] | all files are clean! "

fclean:
	@make -s -C libft fclean
	@make -s -C parse fclean
	@make -s -C execute fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
