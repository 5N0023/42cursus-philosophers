/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlektaib <mlektaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 21:36:18 by mlektaib          #+#    #+#             */
/*   Updated: 2023/02/20 20:56:48 by mlektaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	save_arg(t_arg *param, int argc, char **argv)
{
	int	i;

	i = 0;
	if (ft_atoi(argv[1]) <= 0 || ft_atoi(argv[2]) <= 0 || ft_atoi(argv[3]) <= 0
		|| ft_atoi(argv[4]) <= 0)
		return (printf("argument not valid\n"), exit(1));
	param->number_of_philosophers = ft_atoi(argv[1]);
	param->time_to_die = ft_atoi(argv[2]);
	param->time_to_eat = ft_atoi(argv[3]);
	param->time_to_sleep = ft_atoi(argv[4]);
	param->number_of_philosophers_full = 0;
	if (argc == 6)
	{
		if (ft_atoi(argv[5]) <= 0)
			return (printf("argument not valid\n"), exit(1));
		param->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
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

int	main(int argc, char **argv)
{
	t_arg	param;

	if (argc < 5 || argc > 6)
	{
		printf("not enough argmunets\n");
		exit(1);
	}
	save_arg(&param, argc, argv);
	starprocess(&param);
	return (0);
}
