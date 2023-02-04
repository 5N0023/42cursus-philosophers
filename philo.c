#include "philo.h"


void *philosopher(t_philo *param);
void take_forks(t_philo *param);
void put_forks(t_philo *param);

size_t	get_useconds(void)
{
	struct timeval	time;
	static size_t	starttime;
	gettimeofday(&time, NULL);
	if(!starttime)
		starttime = (time.tv_sec * 1000000 + time.tv_usec)/10000;
	return (((time.tv_sec * 1000000 + time.tv_usec)/10000 - starttime));
}

void	ft_usleep(useconds_t time)
{
	size_t	t;

	t = get_useconds();
	while (get_useconds() - t < time)
		usleep(1);
}

void *ft_controller_dead(t_controller *controller) {

	t_philo *tmp = controller->head;
    while (1) 
	{
		if(tmp)
		{
			if(get_useconds() - tmp->lastmeal > tmp->param->time_to_die)
			{
			pthread_mutex_lock(&controller->param->print);
			printf("%zu %d died \n",get_useconds(),tmp->id);
			exit(0);
			}
		}
		tmp = tmp->right;
		if (!tmp->right)
			tmp = tmp->left;
	}

}

void ft_detachthreads(t_philo *head)
{
	t_philo *tmp;
	t_arg	*param;
	param = head->param;
	int  i  = 0;
	while(i < param->number_of_philosophers)
	{
		pthread_detach(tmp->philo);
		pthread_mutex_destroy(&tmp->fork);
		free(tmp);
		tmp = tmp->right;
		i++;
	}
	pthread_mutex_destroy(&param->print);
}
void *ft_controller(t_controller *controller) {

	t_philo *tmp = controller->head;
    while (1) 
	{
		if(tmp)
		{
			if(tmp->eat_time == tmp->param->number_of_times_each_philosopher_must_eat && tmp->full == 0 && tmp->param->number_of_times_each_philosopher_must_eat )
			{
				tmp->full = 1;
				tmp->param->number_of_philosophers_full++;
			}
			if(tmp->param->number_of_philosophers_full == tmp->param->number_of_philosophers && tmp->param->number_of_times_each_philosopher_must_eat )
			{
			pthread_mutex_lock(&controller->param->print);
			pthread_detach(controller->controller_tread);
			free(controller);
			exit(0);
			}
		}
		tmp = tmp->right;
		if (!tmp->right)
			tmp = tmp->left;
	}
}


void *philosopher(t_philo *philo) {

    while (1) 
	{
		take_forks(philo);
		pthread_mutex_lock(&philo->param->print);
		printf("%zu %d has taken a fork \n",get_useconds(),philo->id);
		pthread_mutex_unlock(&philo->param->print);
		pthread_mutex_lock(&philo->param->print);
		printf("%zu %d is eating \n",get_useconds(),philo->id);
		pthread_mutex_unlock(&philo->param->print);
		philo->eat_time++;
		philo->lastmeal = get_useconds();
		ft_usleep(philo->param->time_to_eat);
		put_forks(philo);
		pthread_mutex_lock(&philo->param->print);
		pthread_mutex_unlock(&philo->param->print);
		printf("%zu %d is sleeping \n",get_useconds(),philo->id);
		ft_usleep(philo->param->time_to_sleep);

    }
}

void take_forks(t_philo *philo) {
	struct timeval current_time;

	pthread_mutex_lock(&philo->fork);
   	pthread_mutex_lock(&philo->right->fork);
}

void put_forks(t_philo *philo) {
	pthread_mutex_unlock(&philo->fork);
   	pthread_mutex_unlock(&philo->right->fork);
	philo->state = SLEEPING;
}



void save_arg(t_arg *param,int argc, char **argv)
{
	int i ;
	i = 0;
	param->number_of_philosophers = atoi(argv[1]);
	param->time_to_die = atoi(argv[2]);
	param->time_to_eat = atoi(argv[3]);
	param->time_to_sleep = atoi(argv[4]);
	param->number_of_philosophers_full =0;
	if (argc == 6)
		param->number_of_times_each_philosopher_must_eat = atoi(argv[5]);
	else
		param->number_of_times_each_philosopher_must_eat = 0;
	
}
int checkargformat(char **argv)
{
	int i;
	int j;

	i = 1;
	j = 0 ;
	while(argv[i])
	{
		while(argv[i][j])
		{
			if(argv[i][j] < '0' || argv[i][j] > '9')
				return 1;
			j++;
		}
		i++;
	}
	return 0;
}

t_philo	*ft_createphilo(int id)
{
	t_philo	*new;
	struct timeval current_time;


	new = malloc(sizeof(t_philo));
	if (!new)
		return (0);
	new->id = id;
	new->right = NULL;
	new->left = NULL;
	new->full = 0;
	new->lastmeal = get_useconds()*1000;
	return (new);
}

void addtolinkedlist(t_philo **head, t_philo *new,int i, int number_of_philosophers)
{
	t_philo *tmp;
	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while(tmp->right)
		tmp = tmp->right;
	tmp->right = new;
	new->left = tmp;
	if(i == number_of_philosophers)
	{
		new->right = *head;
		(*head)->left = new;
	}
}

void	createthreads(t_arg *param)
{
	int i;
	t_philo *tmp;
	t_philo *head = NULL;
	t_controller *controller;
	controller = malloc(sizeof(t_controller));
	tmp = NULL;

	i = 1;
	controller->param = param;
	pthread_mutex_init(&param->print, NULL);
	pthread_create(&controller->controller_tread, NULL, (void *)ft_controller, (void *)controller);
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
	param->start = 0;
	tmp = head;

	while (i <= param->number_of_philosophers)
	{
		
		pthread_create(&tmp->philo, NULL, (void *)philosopher, tmp);
		usleep(50);
		tmp = tmp->right;
		i++;
	}
	i = 0;
	tmp = head;
	while (i < param->number_of_philosophers)
	{
		pthread_join(tmp->philo, NULL);
		tmp = tmp->right;
		i++;
	}
	pthread_join(controller->controller_tread, NULL);
}

int main(int argc, char **argv)
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
	createthreads(&param);

}