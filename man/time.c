/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlektaib <mlektaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 19:36:09 by mlektaib          #+#    #+#             */
/*   Updated: 2023/02/18 20:54:10 by mlektaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	get_ms(t_arg *param)
{
	struct timeval	time;
	static time_t	st;

	pthread_mutex_lock(&param->locktime);
	gettimeofday(&time, NULL);
	if (!st)
		st = ((time.tv_sec * 1000000 + time.tv_usec) / 1000);
	pthread_mutex_unlock(&param->locktime);
	return (((time.tv_sec * 1000000 + time.tv_usec) / 1000 - st));
}

void	ft_usleep(time_t time, t_arg *param)
{
	time_t	t;

	t = get_ms(param);
	while (get_ms(param) - t < time)
		usleep(150);
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
	long	prev;

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
		prev = res;
		res = (*str - 48) + (res * 10);
		if (res / 10 != prev || (sign == 1 && res > 2147483647))
			return (9999999999);
		else if (res / 10 != prev || (sign == -1 && res > 2147483648))
			return (9999999999);
		str++;
	}
	return ((long)res * sign);
}
