SRCS =	man/philo.c man/controller.c man/philolife.c man/pthreads.c man/time.c

SRCSB =	bonus/forks_bonus.c bonus/ft_itoa_bonus.c bonus/philo_bonus.c bonus/philolife_bonus.c bonus/time_bonus.c  bonus/utils_bonus.c

NAME = philo

BONUS = philo_bonus

OBJS = $(SRCS:.c=.o)
OBJSB = $(SRCSB:.c=.o)

MH = man/philo.h
BH = bonus/philo_bonus.h

CC_FLAGS = -Wall -Wextra -Werror

all: $(NAME)

bonus: $(BONUS)

%_bonus.o : %_bonus.c $(BH)
	@cc $(CC_FLAGS) -c $< -o $@

%.o : %.c $(MH)
	@cc $(CC_FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@cc $(OBJS) -o $(NAME)

$(BONUS): $(OBJSB)
	@cc $(OBJSB) -o $(BONUS)
	

clean:
	rm -f $(OBJS) $(OBJSB)

fclean: clean
	rm -f $(NAME) $(BONUS)

re: fclean all

.PHONY : clean fclean re all