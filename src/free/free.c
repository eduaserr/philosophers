/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:01:19 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/13 23:17:53 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

static void ft_free_forks(t_table *table)
{
	int i;

	if (table->forks)
	{
		pthread_mutex_destroy(&table->print_mutex);
		pthread_mutex_destroy(&table->meal_mutex);
		pthread_mutex_destroy(&table->death_mutex);
		i = 0;
		while (i < table->n_ph)
		{
			pthread_mutex_destroy(&table->forks[i]);
			i++;
		}
		free(table->forks);
		table->forks = NULL;
	}
}

void	ft_free_ph(t_table *table)
{
	if (table->philos)
	{
		free(table->philos);
		table->philos = NULL;
	}
}

void	ft_free_all(t_table *table)
{
	ft_free_ph(table);
	ft_free_forks(table);
}
