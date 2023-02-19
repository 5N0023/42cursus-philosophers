/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlektaib <mlektaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 19:40:38 by mlektaib          #+#    #+#             */
/*   Updated: 2023/02/18 20:53:49 by mlektaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

size_t	ft_strlen(const char *str)
{
	size_t	count;

	if (!str)
		return (0);
	count = 0;
	while (str[count])
		count++;
	return (count);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*res;

	if (!s1 || !s2)
		return (0);
	i = 0;
	j = 0;
	res = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (!res)
		return (0);
	while (i < ft_strlen(s1))
	{
		res[i] = s1[i];
		i++;
	}
	while (i < ft_strlen(s1) + ft_strlen(s2))
	{
		res[i] = s2[j];
		i++;
		j++;
	}
	res[ft_strlen(s1) + ft_strlen(s2)] = 0;
	free(s2);
	return (res);
}

void	ft_philo_eat(t_philo *philo)
{
	sem_wait(philo->param->forks);
	sem_wait(philo->param->forks);
	sem_wait(philo->param->print);
	printf("%zu %d has taken a fork \n", get_ms(philo), philo->id);
	printf("%zu %d has taken a fork \n", get_ms(philo), philo->id);
	printf("%zu %d is eating \n", get_ms(philo), philo->id);
	philo->eat_time++;
	philo->lastmeal = get_ms(philo);
	sem_post(philo->param->print);
	ft_usleep(philo->param->time_to_eat, philo);
	sem_post(philo->param->forks);
	sem_post(philo->param->forks);
	if (philo->param->number_of_times_each_philosopher_must_eat
		&& philo->eat_time == philo->param->number_of_times_each_philosopher_must_eat)
		exit(0);
}

void	philosopherselfcheck(t_philo *philo)
{
	while (1)
	{
		sem_wait(philo->param->print);
		if (get_ms(philo) - philo->lastmeal > philo->param->time_to_die)
		{
			printf("%zu %d is died\n", get_ms(philo), philo->id);
			exit(1);
		}
		sem_post(philo->param->print);
		usleep(5000);
	}
}

void	philosopher(t_philo *philo)
{
	pthread_t	controller;
	char		*locktime;
	char		*lock;

	locktime = "/locktime";
	lock = "/lock";
	lock = ft_strjoin(lock, ft_itoa(philo->id));
	locktime = ft_strjoin(locktime, ft_itoa(philo->id));
	sem_unlink(locktime);
	sem_unlink(lock);
	philo->lock = sem_open(lock, O_CREAT, 0600, 1);
	philo->locktime = sem_open(locktime, O_CREAT, 0600, 1);
	if (philo->id % 2)
		ft_usleep(philo->param->time_to_eat, philo);
	pthread_create(&controller, NULL, (void *)philosopherselfcheck, philo);
	while (1)
	{
		ft_philo_eat(philo);
		sem_wait(philo->param->print);
		printf("%zu %d is sleeping \n", get_ms(philo), philo->id);
		sem_post(philo->param->print);
		ft_usleep(philo->param->time_to_sleep, philo);
		sem_wait(philo->param->print);
		printf("%zu %d is thiking \n", get_ms(philo), philo->id);
		sem_post(philo->param->print);
	}
}

static void	createsem(t_arg **param, t_philo **head)
{
	int		i;
	t_philo	*tmp;
	char	*s;
	char	*rac;

	sem_unlink("/print");
	sem_unlink("/forks");
	sem_unlink("/lock");
	i = 1;
	rac = "/";
	i = 1;
	(*param)->print = sem_open("/print", O_CREAT, 0600, 1);
	(*param)->lock = sem_open("/lock", O_CREAT, 0600, 1);
	(*param)->forks = sem_open("/forks", O_CREAT, 0600,
			(*param)->number_of_philosophers);
	tmp = *head;
	while (i <= (*param)->number_of_philosophers)
	{
		tmp = ft_createphilo(i);
		tmp->param = *param;
		addtolinkedlist(head, tmp, i, (*param)->number_of_philosophers);
		i++;
	}
}

static void	createchilds(t_arg **param, t_philo **head)
{
	int		i;
	t_philo	*tmp;
	int		pid;

	i = 1;
	tmp = *head;
	while (i <= (*param)->number_of_philosophers)
	{
		pid = fork();
		tmp->pid = pid;
		if (pid == 0)
		{
			philosopher(tmp);
		}
		tmp = tmp->right;
		i++;
	}
}

void	ft_freeall(t_philo *head)
{
	t_philo	*tmp;
	t_arg	*param;
	int		i;

	param = head->param;
	i = 0;
	tmp = head;
	while (i < param->number_of_philosophers)
	{
		free(tmp);
		tmp = tmp->right;
		i++;
	}
}

int	starprocess(t_arg *param)
{
	int				i;
	t_philo			*tmp;
	t_philo			*head;
	t_controller	*controller;
	int				status;
	char			*rac;
	char			*s;
	int				f;
	int				retpid;
	int				n;

	head = NULL;
	controller = malloc(sizeof(t_controller));
	tmp = NULL;
	i = 1;
	controller->param = param;
	createsem(&param, &head);
	createchilds(&param, &head);
	controller->head = head;
	f = 0;
	while (waitpid(-1, &status, 0))
	{
		if (WIFEXITED(status))
		{
			if (status == 0)
				f++;
			else
				break ;
		}
		if (f == controller->param->number_of_philosophers)
			break ;
	}
	n = 0;
	tmp = head;
	while (n < param->number_of_philosophers)
	{
		kill(tmp->pid, SIGKILL);
		tmp = tmp->right;
		n++;
	}
	ft_freeall(head);
	return (0);
}
