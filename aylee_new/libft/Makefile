NAME = libft.a
DIR = libft
CC = cc -Wall -Wextra -Werror

BOBJS = $(BSRC:.c=.o)
BSRC = ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c ft_lstiter.c ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c

OBJS = $(SRC:.c=.o)
SRC = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c

ifdef WITH_BONUS
	OBJ_FILE = $(OBJS) $(BOBJS)
else
	OBJ_FILE = $(OBJS)
endif

all: $(NAME)
	
$(NAME): $(OBJ_FILE)
	ar -rcs $@ $^

$(DIR)/%.o: $(DIR)/%.c
	$(CC) $< libft.h -o $@

clean:
	rm -f $(OBJS) $(BOBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: 
	@make WITH_BONUS=1 all

.PHONY: clean all fclean re
