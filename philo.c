#include "philo.h"

void	*philosopher(t_philo *param);
void	take_forks(t_philo *param);
void	put_forks(t_philo *param);

size_t	get_useconds(void)
{
	struct timeval	time;
	static size_t	starttime;

	gettimeofday(&time, NULL);
	if (!starttime)
		starttime = ((time.tv_sec * 1000000 + time.tv_usec) / 1000);
	return (((time.tv_sec * 1000000 + time.tv_usec) / 1000 - starttime));
}

void	ft_usleep(useconds_t time)
{
	size_t	t;

	t = get_useconds();
	while (get_useconds() - t < time)
		usleep(200);
}

void	ft_detachthreads(t_philo *head)
{
	t_philo	*tmp;
	t_arg	*param;
	int		i;

	param = head->param;
	i = 0;
	while (i < param->number_of_philosophers)
	{
		pthread_detach(tmp->philo);
		pthread_mutex_destroy(&tmp->fork);
		free(tmp);
		tmp = tmp->right;
		i++;
	}
	pthread_mutex_destroy(&param->print);
}

int	ft_controller(t_controller *controller)
{
	t_philo	*tmp;
	time_t	time;
	int		i;

	tmp = controller->head;
	i = 0;
	time = get_useconds();
	i = 0;
	while (i < tmp->param->number_of_philosophers)
	{
		if (tmp->eat_time == tmp->param->number_of_times_each_philosopher_must_eat
			&& tmp->full == 0
			&& tmp->param->number_of_times_each_philosopher_must_eat != 0)
		{
			tmp->full = 1;
			tmp->param->number_of_philosophers_full++;
		}
		if (tmp->param->number_of_philosophers_full == tmp->param->number_of_philosophers
			&& tmp->param->number_of_times_each_philosopher_must_eat != 0)
		{
			tmp->param->state = GAMEOVER;
			pthread_mutex_lock(&controller->param->print);
			pthread_detach(controller->controller_tread);
			free(controller);
			return (1);
		}
		if (time - tmp->lastmeal > tmp->param->time_to_die)
		{
			tmp->param->state = GAMEOVER;
			pthread_mutex_lock(&controller->param->print);
			printf("%zu %d died\n", time, tmp->id);
			pthread_detach(controller->controller_tread);
			free(controller);
			return (1);
		}
		tmp = tmp->right;
		i++;
	}
	return (0);
}

void	*philosopher(t_philo *philo)
{
	while (1)
	{
		if (philo->param->state == GAMEOVER)
			return (NULL);
		take_forks(philo);
		philo->lastmeal = get_useconds();
		pthread_mutex_lock(&philo->param->print);
		printf("%zu %d has taken a fork \n", get_useconds(), philo->id);
		printf("%zu %d is eating \n", get_useconds(), philo->id);
		pthread_mutex_unlock(&philo->param->print);
		philo->eat_time++;
		ft_usleep(philo->param->time_to_eat);
		put_forks(philo);
		pthread_mutex_lock(&philo->param->print);
		printf("%zu %d is sleeping \n", get_useconds(), philo->id);
		pthread_mutex_unlock(&philo->param->print);
		ft_usleep(philo->param->time_to_sleep);
		pthread_mutex_lock(&philo->param->print);
		printf("%zu %d is thiking \n", get_useconds(), philo->id);
		pthread_mutex_unlock(&philo->param->print);
	}
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork);
	pthread_mutex_lock(&philo->left->fork);
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(&philo->left->fork);
}

void	save_arg(t_arg *param, int argc, char **argv)
{
	int	i;

	i = 0;
	if (atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0 || atoi(argv[3]) <= 0
		|| atoi(argv[4]) <= 0)
		return (printf("argument not valid\n"), exit(1));
	param->number_of_philosophers = atoi(argv[1]);
	param->time_to_die = atoi(argv[2]);
	param->time_to_eat = atoi(argv[3]);
	param->time_to_sleep = atoi(argv[4]);
	param->number_of_philosophers_full = 0;
	param->state = 0;
	if (argc == 6)
	{
		if (atoi(argv[5]) <= 0)
			return (printf("argument not valid\n"), exit(1));
		param->number_of_times_each_philosopher_must_eat = atoi(argv[5]);
	}
	else
		param->number_of_times_each_philosopher_must_eat = 0;
}
int	checkargformat(char **argv)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (argv[i])
	{
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

t_philo	*ft_createphilo(int id)
{
	t_philo			*new;
	struct timeval	current_time;

	new = malloc(sizeof(t_philo));
	if (!new)
		return (0);
	new->id = id;
	new->right = NULL;
	new->left = NULL;
	new->forksatate = FREEFORK;
	new->full = 0;
	new->lastmeal = 0;
	return (new);
}

void	addtolinkedlist(t_philo **head, t_philo *new, int i,
		int number_of_philosophers)
{
	t_philo	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->right)
		tmp = tmp->right;
	tmp->right = new;
	new->left = tmp;
	if (i == number_of_philosophers)
	{
		new->right = *head;
		(*head)->left = new;
	}
}

int	createthreads(t_arg *param)
{
	int				i;
	t_philo			*tmp;
	t_philo			*head;
	t_controller	*controller;

	head = NULL;
	controller = malloc(sizeof(t_controller));
	tmp = NULL;
	i = 1;
	controller->param = param;
	pthread_mutex_init(&param->print, NULL);
	while (i <= param->number_of_philosophers)
	{
		tmp = ft_createphilo(i);
		tmp->param = param;
		addtolinkedlist(&head, tmp, i, param->number_of_philosophers);
		controller->head = head;
		pthread_mutex_init(&tmp->fork, NULL);
		i++;
	}
	i = 1;
	tmp = head;
	while (i <= param->number_of_philosophers)
	{
		pthread_create(&tmp->philo, NULL, (void *)philosopher, tmp);
		usleep(200);
		tmp = tmp->right;
		i++;
	}
	i = 0;
	tmp = head;
	while (1)
	{
		if (ft_controller(controller))
			return (1);
		usleep(10000);
	}
	while (i < param->number_of_philosophers)
	{
		pthread_join(tmp->philo, NULL);
		tmp = tmp->right;
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_arg param;
	t_philo *head;
	pthread_t *forks;

	if (argc < 5 || argc > 6)
	{
		printf("not enough argmunets\n");
		exit(1);
	}
	save_arg(&param, argc, argv);
	if (atoi(argv[1]) == 1)
	{
		printf("0 1 is thinking\n");
		ft_usleep(atoi(argv[2]));
		printf("%d 1 died\n", atoi(argv[2]));
		exit(0);
	}
	if (createthreads(&param) == 1)
		return (0);
}