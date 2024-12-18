NAME     = a.out
CC       = cc
#CFLAGS   = -Wall -Wextra -Werror -g -fsanitize=address
CFLAGS   = -Wall -Wextra -Werror
SRCS     = microshell.c
OBJS     = $(SRCS:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

test: re
	./$(NAME) /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell

.PHONY: all clean fclean re test
