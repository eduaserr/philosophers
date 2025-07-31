/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 18:45:46 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/31 14:19:30 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

int	one_ph(t_philo *ph)
{
	if (ph->table->n_ph == 1)
	{
		print_msg(ph, "has taken a fork");
		usleep(ph->table->time_to_die * 1000);
		return (1);
	}
	return (0);
}

void	*ph_routine(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	if (one_ph(ph))
		return (NULL);
	if (ph->id % 2 == 1)
		usleep(100);
	pthread_mutex_lock(&ph->table->meal_mutex);
	ph->last_meal = get_time();
	pthread_mutex_unlock(&ph->table->meal_mutex);
	while (!check_someone_died(ph->table) && !check_meals(ph))
	{
		if (eat(ph))
			break ;
		if (ft_sleep(ph))
			break ;
		if (think(ph))
			break ;
	}
	return (NULL);
}

static int	check_all_deaths(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->n_ph)
	{
		if (check_death(&table->philos[i]))
		{
			someone_died(table);
			pthread_mutex_lock(&table->print_mutex);
			printf("%ld %d died\n", get_timestamp(table),
				table->philos[i].id + 1);
			pthread_mutex_unlock(&table->print_mutex);
			return (1);
		}
	}
	return (0);
}

static int	check_all_meals(t_table *table)
{
	int	i;
	int	all_done;

	if (table->n_meals == -1)
		return (0);
	all_done = 1;
	i = -1;
	while (++i < table->n_ph)
	{
		if (!check_meals(&table->philos[i]))
		{
			all_done = 0;
			break ;
		}
	}
	if (all_done)
	{
		someone_died(table);
		return (1);
	}
	return (0);
}

void	*dh_routine(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	while (!check_someone_died(table))
	{
		if (check_all_deaths(table))
			return (NULL);
		if (check_all_meals(table))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
