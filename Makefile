SRCS =	philo.c

NAME = philo

OBJS = $(SRCS:.c=.o)

CC_FLAGS = -Wall -Wextra -Werror

# %_bonus.o : %.c philo_bonus.h
# 	@cc  -c $<

%.o : %.c philo.h
	@cc  -c $<

$(NAME): $(OBJS)
	@cc $(OBJS) -o $(NAME)
all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY : clean fclean re all