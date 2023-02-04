#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define SLEEPING 0
#define THINKING 1
#define EATING 2


typedef struct s_arg
{
	int number_of_philosophers;
	int time_to_die ;
	int time_to_eat;
	int time_to_sleep;
	int number_of_times_each_philosopher_must_eat;
	int number_of_philosophers_full;
	pthread_mutex_t print;
	int start;
} t_arg;

typedef struct s_philo
{
	pthread_t philo;
	pthread_mutex_t fork;
	int id;
	int lastmeal;
	int state;
	int eat_time;
	int full;
	struct s_arg *param;
	struct s_philo *right;
	struct s_philo *left;
} t_philo;

typedef struct s_controller
{
	pthread_t controller_tread;
	t_philo *head;
	t_arg	*param;
} t_controller;

#endif