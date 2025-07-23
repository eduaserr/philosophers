/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 17:34:56 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/23 17:42:05 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

void	someone_died(t_table *table)
{
	pthread_mutex_lock(&table->death_mutex);
	table->someone_died = 1;
	pthread_mutex_unlock(&table->death_mutex);
}

int	check_someone_died(t_table *table)
{
	int	died;

	pthread_mutex_lock(&table->death_mutex);
	died = table->someone_died;
	pthread_mutex_unlock(&table->death_mutex);
	return (died);
}

int	check_death(t_philo *ph)
{
	long	current_time;
	long	last_meal;

	pthread_mutex_lock(&ph->table->meal_mutex);
	current_time = get_time();
	last_meal = ph->last_meal;
	pthread_mutex_unlock(&ph->table->meal_mutex);
	if ((current_time - last_meal) >= ph->table->time_to_die)
		return (1);
	return (0);
}

int	check_meals(t_philo *ph)
{
	int	meals;

	if (ph->table->n_meals == -1)
		return (0);
	pthread_mutex_lock(&ph->table->meal_mutex);
	meals = ph->meals;
	pthread_mutex_unlock(&ph->table->meal_mutex);
	if (meals >= ph->table->n_meals)
		return (1);
	return (0);
}
