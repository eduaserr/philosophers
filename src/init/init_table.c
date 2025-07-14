/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 01:18:24 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/14 19:17:34 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

static void	init_philos(int id, t_table *table, t_philo *ph)
{
	ph->id = id;
	ph->meals = 0;
	ph->last_meal = 0;
	ph->l_fork = &table->forks[id];
	ph->r_fork = &table->forks[(id + 1) % table->n_ph];
	ph->thread = 0;
	ph->table = table;
}

static int	init_forks(int id, t_table *table)
{
	if (pthread_mutex_init(&table->forks[id], NULL) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	init_helpu(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_ph)
	{
		if (init_forks(i, table) == 1)
			return (ft_perror("init fork"), 1);
        init_philos(i, table, &table->philos[i]);
		if (!table->philos)
			return (ft_perror("init philo"), 1);
		i++;
	}
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&table->meal_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&table->death_mutex, NULL) != 0)
		return (1);
	return (0);
}

void	init_table(t_table *table, char **av)
{
	table->n_ph = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	table->n_meals = -1;
	if (av[5])
		table->n_meals = ft_atoi(av[5]);
	table->philos = NULL;
	table->forks = NULL;
	table->philos = (t_philo *)malloc(sizeof(t_philo) * table->n_ph);
	if (!table->philos)
		return (ft_perror("philo struct"));
	table->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * table->n_ph);
	if (!table->forks)
		return (ft_free_ph(table), ft_perror("forks struct"));
	if (init_helpu(table) == 1)
		return (ft_free_all(table), ft_perror("mutex init"));
	table->start_time = 0;
}
