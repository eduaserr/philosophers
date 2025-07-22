/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/22 22:50:39 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

static int	get_forks(t_philo *ph)
{
	if (ph->id % 2 == 0)
	{
		if (pthread_mutex_lock(ph->l_fork) != 0)
			return (1);
		print_msg(ph, "l_fork");
		if (pthread_mutex_lock(ph->r_fork) != 0)
		{
			pthread_mutex_unlock(ph->l_fork);
			return (1);
		}
		print_msg(ph, "r_fork");
	}
	else
	{
		if (pthread_mutex_lock(ph->r_fork) != 0)
			return (1);
		print_msg(ph, "r_fork");
		if (pthread_mutex_lock(ph->l_fork) != 0)
		{
			pthread_mutex_unlock(ph->r_fork);
			return (1);
		}
		print_msg(ph, "l_fork");
	}
	return (0);
}

static int	ft_lastmeal_mutex(t_philo *ph)
{
	if (pthread_mutex_lock(&ph->table->meal_mutex) != 0)
		return (1);
	ph->last_meal = get_time();
	ph->meals++;
	if (pthread_mutex_unlock(&ph->table->meal_mutex) != 0)
		return (1);
	return (0);
}

int	ph_sleep(t_philo *ph, long time)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < time)
	{
		if (check_someone_died(ph->table))
			return (1);
		usleep(500);
	}
	return (0);
}

int	eat(t_philo *ph)
{
	if (get_forks(ph) == 1)
		return (1);
	print_msg(ph, "eating");
	if (ph_sleep(ph, ph->table->time_to_eat))
	{
		pthread_mutex_unlock(ph->l_fork);
		pthread_mutex_unlock(ph->r_fork);
		return (1);
	}
	ft_lastmeal_mutex(ph);
	printf("ultima comida = %ld, nº comida %d\n", ph->last_meal, ph->meals);
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
		usleep(500);
	}
	return (0);
}
