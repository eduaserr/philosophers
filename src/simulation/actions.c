/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/17 18:28:36 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

static int	get_forks(t_philo *ph)
{
	if (ph->id % 2 == 0)
	{
		if (pthread_mutex_lock(ph->l_fork) != 0)
			return (1);
		if (print_msg(ph, "l_fork") != 0)
			return (1);
		if (pthread_mutex_lock(ph->r_fork) != 0)
			return (1);
		if (print_msg(ph, "r_fork") != 0)
			return (1);
	}
	else
	{
		if (pthread_mutex_lock(ph->r_fork) != 0)
			return (1);
		if (print_msg(ph, "r_fork") != 0)
			return (1);
		if (pthread_mutex_lock(ph->l_fork) != 0)
			return (1);
		if (print_msg(ph, "l_fork") != 0)
			return (1);
	}
	return (0);
}

int	eat(t_philo *ph)
{
	if (get_forks(ph) == 1)
		return (1);
	if (print_msg(ph, "eating"))
		return (1);
	if (pthread_mutex_lock(&ph->table->meal_mutex))
		return (1);
	ph->last_meal = get_time();
	ph->meals++;
	if (pthread_mutex_unlock(&ph->table->meal_mutex))
		return (1);
	usleep(ph->table->time_to_eat * 1000);
	if (pthread_mutex_unlock(ph->l_fork))
		return (1);
	if (pthread_mutex_unlock(ph->r_fork))
		return (1);
	return (0);
}

int	think(t_philo *ph)
{
	(void)ph;
	if (print_msg(ph, "thinking") != 0)
		return (1);
	return (0);
}

int	ft_sleep(t_philo *ph)
{
	(void)ph;
	if (print_msg(ph, "sleeping") != 0)
		return (1);
	usleep(ph->table->time_to_sleep * 1000);
	return (0);
}
