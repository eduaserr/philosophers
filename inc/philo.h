#ifndef PHILO_H
#define PHILO_H

# define E_MSG "\x1b[31m%s\x1B[37m%s\n", "Error : "

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_philo
{
	int				id;				//identificative number of the philo
	int				meals;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	struct t_table	*table;
}	t_philo;

typedef struct s_table
{
	int				n_ph;			//how many philos
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_meals;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	death_mutex;
	t_philo			*philos;
}	t_table;

/* **************************************** */
/*					UTILS					*/
/* **************************************** */
//////////////////////
//	utils.c			//
//////////////////////
void	ft_perror(t_table *table, char *msg, int code);

#endif