/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlektaib <mlektaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 19:37:21 by mlektaib          #+#    #+#             */
/*   Updated: 2023/02/19 19:23:40 by mlektaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_detachthreads(t_philo *head)
{
	t_philo	*tmp;
	t_arg	*param;
	int		i;

	param = head->param;
	i = 0;
	tmp = head;
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

static void	ft_checkeattimes(t_philo *tmp)
{
	pthread_mutex_lock(&tmp->param->lock);
	if (tmp->eat_time == tmp->param->number_of_times_each_philosopher_must_eat
		&& tmp->full == 0
		&& tmp->param->number_of_times_each_philosopher_must_eat != 0)
	{
		tmp->full = 1;
		tmp->param->number_of_philosophers_full++;
	}
	pthread_mutex_unlock(&tmp->param->lock);
}

static int	ft_checkifalleat(t_philo *tmp, t_controller *controller)
{
	pthread_mutex_lock(&tmp->param->lock);
	if (tmp->param->number_of_philosophers_full
		== tmp->param->number_of_philosophers
		&& tmp->param->number_of_times_each_philosopher_must_eat != 0)
	{
		pthread_mutex_lock(&controller->param->print);
		ft_detachthreads(controller->head);
		free(controller);
		return (1);
	}
	pthread_mutex_unlock(&tmp->param->lock);
	return (0);
}

static int	ft_checkdeath(t_philo *tmp, t_controller *controller, time_t time)
{
	pthread_mutex_lock(&tmp->param->lock);
	if (time - tmp->lastmeal >= tmp->param->time_to_die)
	{
		pthread_mutex_lock(&controller->param->print);
		ft_detachthreads(controller->head);
		ft_usleep(controller->param->number_of_philosophers * 10,
			controller->param);
		printf("%zu %d died\n", time, tmp->id);
		free(controller);
		return (1);
	}
	pthread_mutex_unlock(&tmp->param->lock);
	return (0);
}

int	ft_controller(t_controller *controller)
{
	t_philo	*tmp;
	time_t	time;
	int		i;
	int		nbr;

	i = 0;
	tmp = controller->head;
	nbr = tmp->param->number_of_philosophers;
	time = get_ms(tmp->param);
	while (i < nbr)
	{
		ft_checkeattimes(tmp);
		if (ft_checkifalleat(tmp, controller))
		{
			return (1);
		}
		if (ft_checkdeath(tmp, controller, time))
		{
			return (1);
		}
		tmp = tmp->right;
		i++;
	}
	return (0);
}
