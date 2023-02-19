/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthreads.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlektaib <mlektaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 19:40:38 by mlektaib          #+#    #+#             */
/*   Updated: 2023/02/18 20:54:08 by mlektaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	createmutex(t_arg **param, t_philo **head)
{
	int		i;
	t_philo	*tmp;

	i = 1;
	pthread_mutex_init(&(*param)->print, NULL);
	pthread_mutex_init(&(*param)->lock, NULL);
	pthread_mutex_init(&(*param)->locktime, NULL);
	pthread_mutex_init(&(*param)->start, NULL);
	tmp = *head;
	while (i <= (*param)->number_of_philosophers)
	{
		tmp = ft_createphilo(i);
		tmp->param = *param;
		addtolinkedlist(head, tmp, i, (*param)->number_of_philosophers);
		pthread_mutex_init(&tmp->fork, NULL);
		i++;
	}
}

static void	createthreads(t_arg **param, t_philo **head)
{
	int		i;
	t_philo	*tmp;

	i = 1;
	tmp = *head;
	while (i <= (*param)->number_of_philosophers)
	{
		tmp->param = (*param);
		pthread_create(&tmp->philo, NULL, (void *)philosopher, tmp);
		tmp = tmp->right;
		i++;
	}
	tmp = *head;
	i = 0;
}

int	starprocess(t_arg *param)
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
	createmutex(&param, &head);
	createthreads(&param, &head);
	controller->head = head;
	while (1)
	{
		if (ft_controller(controller))
			return (1);
		usleep(10000);
	}
}
