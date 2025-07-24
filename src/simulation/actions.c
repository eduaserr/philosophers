/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/24 18:46:32 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

static int	odd_fork(t_philo *ph)
{
	if (pthread_mutex_lock(ph->r_fork) != 0)
		return (1);
	print_msg(ph, "r_fork");
	if (check_someone_died(ph->table))
	{
		pthread_mutex_unlock(ph->r_fork);
		return (1);
	}
	if (pthread_mutex_lock(ph->l_fork) != 0)
	{
		pthread_mutex_unlock(ph->r_fork);
		return (1);
	}
	print_msg(ph, "l_fork");
	return (0);
}

static int	get_forks(t_philo *ph)
{
	if (check_someone_died(ph->table))
		return (1);
	if (ph->id % 2 == 0)
	{
		if (pthread_mutex_lock(ph->l_fork) != 0)
			return (1);
		print_msg(ph, "l_fork");
		if (check_someone_died(ph->table))
		{
			pthread_mutex_unlock(ph->l_fork);
			return (1);
		}
		if (pthread_mutex_lock(ph->r_fork) != 0)
		{
			pthread_mutex_unlock(ph->l_fork);
			return (1);
		}
		print_msg(ph, "r_fork");
	}
	else
		return (odd_fork(ph));
	return (0);
}

int	eat(t_philo *ph)
{
	if (check_someone_died(ph->table))
		return (1);
	if (get_forks(ph) == 1)
		return (1);
	print_msg(ph, "eating");
	ft_lastmeal_mutex(ph);
	if (ph_sleep(ph, ph->table->time_to_eat))
	{
		pthread_mutex_unlock(ph->l_fork);
		pthread_mutex_unlock(ph->r_fork);
		return (1);
	}
	pthread_mutex_unlock(ph->l_fork);
	pthread_mutex_unlock(ph->r_fork);
	if (check_someone_died(ph->table))
		return (1);
	return (0);
}

int	think(t_philo *ph)
{
	long	current_time;
	long	time_since_meal;
	long	think_time;

	if (check_someone_died(ph->table))
		return (1);
	current_time = get_time();
	pthread_mutex_lock(&ph->table->meal_mutex);
	time_since_meal = current_time - ph->last_meal;
	pthread_mutex_unlock(&ph->table->meal_mutex);
	think_time = (ph->table->time_to_die - time_since_meal - ph->table->time_to_eat) / 2;
	if (think_time < 1)
		think_time = 1;
	if (think_time > 50)
		think_time = 50;
	print_msg(ph, "is thinking");
	return (usleep(think_time));
}

int	ft_sleep(t_philo *ph)
{
	long	start;

	if (check_someone_died(ph->table))
		return (1);
	if (print_msg(ph, "sleeping"))
		return (1);
	start = get_time();
	while (get_time() - start < ph->table->time_to_sleep)
	{
		if (check_someone_died(ph->table))
			return (1);
		usleep(10);
	}
	return (0);
}
