/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlektaib <mlektaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 17:15:33 by mlektaib          #+#    #+#             */
/*   Updated: 2023/02/19 19:24:26 by mlektaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_arg
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	int				number_of_philosophers_full;
	pthread_mutex_t	print;
	pthread_mutex_t	lock;
	pthread_mutex_t	locktime;
	pthread_mutex_t	start;
}					t_arg;

typedef struct s_philo
{
	pthread_t		philo;
	pthread_mutex_t	fork;
	int				id;
	int				lastmeal;
	int				state;
	int				eat_time;
	int				full;
	struct s_arg	*param;
	struct s_philo	*right;
	struct s_philo	*left;
}					t_philo;

typedef struct s_controller
{
	pthread_t		controller_tread;
	t_philo			*head;
	t_arg			*param;
}					t_controller;

long				ft_atoi(const char *str);
void				ft_usleep(time_t time, t_arg *param);
time_t				get_ms(t_arg *param);
int					starprocess(t_arg *param);
int					checkargformat(char **argv);
void				save_arg(t_arg *param, int argc, char **argv);
int					ft_controller(t_controller *controller);
t_philo				*ft_createphilo(int id);
void				addtolinkedlist(t_philo **head, t_philo *new, int i,
						int number_of_philosophers);
void				philosopher(t_philo *philo);
#endif