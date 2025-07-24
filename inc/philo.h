/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 20:53:49 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/24 20:54:15 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define E_MSG "\x1b[31mError : \x1B[37m%s\n"

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>

typedef struct s_table	t_table;

typedef struct s_philo
{
	int				id;
	int				meals;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	t_table			*table;
}	t_philo;

typedef struct s_table
{
	int				n_ph;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				n_meals;
	long			start_time;
	int				someone_died;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	death_mutex;
	t_philo			*philos;
}	t_table;

/* **************************************** */
/*					LIB						*/
/* **************************************** */
int		ft_atoi(const char *str);
int		ft_isdigit(int a);
int		ft_strcmp(const char *s1, const char *s2);

/* **************************************** */
/*					FREE					*/
/* **************************************** */
//////////////////////
//	free.c			//
//////////////////////
void	ft_free_ph(t_table *table);
void	ft_free_all(t_table *table);

/* **************************************** */
/*					INIT					*/
/* **************************************** */
//////////////////////
//	init_parser.c	//
//////////////////////
int		check_args(int ac, char **av);

//////////////////////
//	init_table.c	//
//////////////////////
void	init_table(t_table *table, char **av);

/* **************************************** */
/*					SIMULATION				*/
/* **************************************** */
//////////////////////
//	actions.c		//
//////////////////////
int		eat(t_philo *ph);
int		think(t_philo *ph);
int		ft_sleep(t_philo *ph);

//////////////////////
//	routines.c		//
//////////////////////
void	*ph_routine(void *arg);
void	*dh_routine(void *arg);

//////////////////////
//	routine_utils.c	//
//////////////////////
int		check_death(t_philo *ph);
void	someone_died(t_table *table);
int		check_someone_died(t_table *table);
int		check_meals(t_philo *ph);

//////////////////////
//	time.c			//
//////////////////////
long	get_time(void);
long	get_timestamp(t_table *table);

/* **************************************** */
/*					UTILS					*/
/* **************************************** */
//////////////////////
//	utils_error.c	//
//////////////////////
void	ft_perror(char *msg);

//////////////////////
//	utils_print.c	//
//////////////////////
int		print_msg(t_philo *ph, char *msg);

//////////////////////
//	utils.c			//
//////////////////////
int		ft_lastmeal_mutex(t_philo *ph);
int		ph_sleep(t_philo *ph, long time);

#endif