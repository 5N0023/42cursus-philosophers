/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlektaib <mlektaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 19:40:38 by mlektaib          #+#    #+#             */
/*   Updated: 2023/02/20 20:56:28 by mlektaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	createsem(t_arg **param, t_philo **head)
{
	int		i;
	t_philo	*tmp;

	sem_unlink("/print");
	sem_unlink("/forks");
	sem_unlink("/lock");
	sem_unlink("/locktime");
	i = 1;
	(*param)->print = sem_open("/print", O_CREAT, 0600, 1);
	(*param)->lock = sem_open("/lock", O_CREAT, 0600, 1);
	(*param)->locktime = sem_open("/locktime", O_CREAT, 0600, 1);
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
	t_philo			*tmp;
	t_philo			*head;
	t_controller	*controller;
	int				n;

	head = NULL;
	controller = malloc(sizeof(t_controller));
	tmp = NULL;
	controller->param = param;
	createsem(&param, &head);
	createchilds(&param, &head);
	controller->head = head;
	waiteventsfromphilo(controller);
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
