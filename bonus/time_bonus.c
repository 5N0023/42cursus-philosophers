/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlektaib <mlektaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 19:36:09 by mlektaib          #+#    #+#             */
/*   Updated: 2023/02/18 20:53:44 by mlektaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

time_t	get_ms(t_philo *philo)
{
	struct timeval	time;
	static time_t	st;

	sem_wait(philo->locktime);
	gettimeofday(&time, NULL);
	if (!st)
		st = ((time.tv_sec * 1000000 + time.tv_usec) / 1000);
	sem_post(philo->locktime);
	return (((time.tv_sec * 1000000 + time.tv_usec) / 1000 - st));
}

void	ft_usleep(time_t time, t_philo *philo)
{
	time_t	t;

	t = get_ms(philo);
	while (get_ms(philo) - t < time)
		usleep(200);
}

static int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r'
		|| c == '\n');
}

static int	ft_isdigit(int n)
{
	return (n >= '0' && n <= '9');
}

long	ft_atoi(const char *str)
{
	size_t	res;
	int		sign;

	sign = 1;
	res = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		res = (*str - 48) + (res * 10);
		str++;
	}
	return ((long)res * sign);
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
t_philo	*ft_createphilo(int id)
{
	t_philo *new;

	new = malloc(sizeof(t_philo));
	if (!new)
		return (0);
	new->id = id;
	new->right = NULL;
	new->left = NULL;
	new->eat_time = 0;
	new->lastmeal = 0;
	return (new);
}