/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 20:43:07 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/23 17:35:29 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

int	ft_lastmeal_mutex(t_philo *ph)
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
		usleep(100);
	}
	return (0);
}
