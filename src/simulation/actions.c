/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/31 14:23:07 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

// Tiempo restante antes de morir
long	time_left_to_die(t_philo *ph)
{
	long	now;
	long	last_meal;

	pthread_mutex_lock(&ph->table->meal_mutex);
	last_meal = ph->last_meal;
	pthread_mutex_unlock(&ph->table->meal_mutex);
	now = get_time();
	return (ph->table->time_to_die - (now - last_meal));
}

//
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
	if (check_someone_died(ph->table))
		return (1);
	if (print_msg(ph, "thinking") != 0)
		return (1);
	usleep(((ph->table->time_to_eat + ph->table->time_to_sleep) / 2) * 1000);
	return (0);
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
