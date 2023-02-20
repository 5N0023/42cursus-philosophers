/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philolife_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlektaib <mlektaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 20:09:44 by mlektaib          #+#    #+#             */
/*   Updated: 2023/02/20 20:21:12 by mlektaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	philosopherselfcheck(t_philo *philo)
{
	while (1)
	{
		sem_wait(philo->param->lock);
		if (get_ms(philo->param) - philo->lastmeal > philo->param->time_to_die)
		{
			sem_wait(philo->param->print);
			printf("%zu %d is died\n", get_ms(philo->param), philo->id);
			exit(1);
		}
		sem_post(philo->param->lock);
		usleep(5000);
	}
}

void	ft_philo_eat(t_philo *philo)
{
	sem_wait(philo->param->forks);
	sem_wait(philo->param->forks);
	sem_wait(philo->param->print);
	printf("%zu %d has taken a fork \n", get_ms(philo->param), philo->id);
	printf("%zu %d has taken a fork \n", get_ms(philo->param), philo->id);
	printf("%zu %d is eating \n", get_ms(philo->param), philo->id);
	sem_post(philo->param->print);
	sem_wait(philo->param->lock);
	philo->eat_time++;
	philo->lastmeal = get_ms(philo->param);
	sem_post(philo->param->lock);
	myusleep(philo->param->time_to_eat, philo->param);
	sem_post(philo->param->forks);
	sem_post(philo->param->forks);
	if (philo->param->number_of_times_each_philosopher_must_eat
		&& philo->eat_time
		== philo->param->number_of_times_each_philosopher_must_eat)
		exit(0);
}

void	philosopher(t_philo *philo)
{
	pthread_t	controller;

	if (philo->id % 2)
		myusleep(philo->param->time_to_eat, philo->param);
	pthread_create(&controller, NULL, (void *)philosopherselfcheck, philo);
	while (1)
	{
		ft_philo_eat(philo);
		sem_wait(philo->param->print);
		printf("%zu %d is sleeping \n", get_ms(philo->param), philo->id);
		sem_post(philo->param->print);
		myusleep(philo->param->time_to_sleep, philo->param);
		sem_wait(philo->param->print);
		printf("%zu %d is thiking \n", get_ms(philo->param), philo->id);
		sem_post(philo->param->print);
	}
}
