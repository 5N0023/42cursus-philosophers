/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philolife.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlektaib <mlektaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 19:40:21 by mlektaib          #+#    #+#             */
/*   Updated: 2023/02/20 19:33:08 by mlektaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*ft_createphilo(int id)
{
	t_philo	*new;

	new = malloc(sizeof(t_philo));
	if (!new)
		return (0);
	new->id = id;
	new->right = NULL;
	new->left = NULL;
	new->full = 0;
	new->eat_time = 0;
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

void	ft_philo_eat(t_philo *philo)
{
	time_t	time;

	pthread_mutex_lock(&philo->fork);
	pthread_mutex_lock(&philo->param->print);
	printf("%zu %d has taken a fork \n", get_ms(philo->param), philo->id);
	pthread_mutex_unlock(&philo->param->print);
	pthread_mutex_lock(&philo->right->fork);
	pthread_mutex_lock(&philo->param->print);
	printf("%zu %d has taken a fork \n", get_ms(philo->param), philo->id);
	pthread_mutex_unlock(&philo->param->print);
	pthread_mutex_lock(&philo->param->print);
	time = get_ms(philo->param);
	printf("%zu %d is eating \n", time, philo->id);
	pthread_mutex_unlock(&philo->param->print);
	pthread_mutex_lock(&philo->param->lock);
	philo->eat_time++;
	philo->lastmeal = get_ms(philo->param);
	pthread_mutex_unlock(&philo->param->lock);
	myusleep(philo->param->time_to_eat, philo->param);
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(&philo->right->fork);
}

void	philosopher(t_philo *philo)
{
	if (philo->param->number_of_philosophers == 1)
	{
		printf("%zu %d is thiking \n", get_ms(philo->param), philo->id);
		return ;
	}
	if (philo->id % 2)
		myusleep(philo->param->time_to_eat - 50, philo->param);
	while (1)
	{
		ft_philo_eat(philo);
		pthread_mutex_lock(&philo->param->print);
		printf("%zu %d is sleeping \n", get_ms(philo->param), philo->id);
		pthread_mutex_unlock(&philo->param->print);
		myusleep(philo->param->time_to_sleep, philo->param);
		pthread_mutex_lock(&philo->param->print);
		printf("%zu %d is thiking \n", get_ms(philo->param), philo->id);
		pthread_mutex_unlock(&philo->param->print);
	}
}
