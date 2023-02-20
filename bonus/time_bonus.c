/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlektaib <mlektaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 19:36:09 by mlektaib          #+#    #+#             */
/*   Updated: 2023/02/20 20:16:55 by mlektaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

time_t	get_ms(t_arg *philo)
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

void	myusleep(time_t time, t_arg *param)
{
	time_t	t;

	t = get_ms(param);
	while (get_ms(param) - t < time)
		usleep(150);
}

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

void	waiteventsfromphilo(t_controller *controller)
{
	int	f;
	int	status;

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
}
