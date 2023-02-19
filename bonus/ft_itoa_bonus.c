/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlektaib <mlektaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:55:12 by mlektaib          #+#    #+#             */
/*   Updated: 2023/02/18 20:53:48 by mlektaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	ft_countdigit(int n)
{
	int	count;
	int	num;

	count = 0;
	if (n == 0)
		n = 1;
	num = n;
	while (num != 0)
	{
		count++;
		num = num / 10;
	}
	return (count);
}

static char	*ft_itoahelper(int count, char *tab, int i, long num)
{
	tab[count] = '\0';
	count--;
	while (count >= i)
	{
		tab[count] = (num % 10) + '0';
		num = num / 10;
		count--;
	}
	return (tab);
}

char	*ft_itoa(int n)
{
	int count;
	char *tab;
	int i;
	long num;

	i = 0;
	num = n;
	count = ft_countdigit(n);
	if (n < 0)
		count++;
	tab = malloc(sizeof(char) * count + 1);
	if (!tab)
		return (0);
	if (n < 0)
	{
		tab[i] = '-';
		i++;
		num = -num;
	}
	return (ft_itoahelper(count, tab, i, num));
}