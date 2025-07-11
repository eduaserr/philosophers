/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 01:18:24 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/11 14:49:59 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

/* void	ft_free_philos(t_philo ***ph)
{
	free(**ph);
} */

t_philo	*init_philos(int id, t_table *table)
{
	t_philo	*ph;

	ph = (t_philo *)malloc(sizeof(t_philo));
	if (!ph)
		return (NULL);
	ph->id = id;
	ph->meals = 0;
	ph->last_meal = 0;
	ph->l_fork = &table->forks[id];
	ph->r_fork = &table->forks[(id + 1) % table->n_ph];
	ph->thread = 0;
	ph->table = table;
	return (ph);
}

pthread_mutex_t *init_forks(int id, t_table *table)
{
	pthread_mutex_t *fk;
	int	i;


	fk = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (!fk)
		return (NULL);
	i = 0;
	/* INVESTIGAR POR QUÉ SE INICIAN ASÍ LOS TENEDORES
	if (pthread_mutex_init(table->forks, NULL) != 0)
		return (NULL);
	*/
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (NULL);
	if (pthread_mutex_init(&table->meal_mutex, NULL) != 0)
		return (NULL);
	if (pthread_mutex_init(&table->death_mutex, NULL) != 0)
		return (NULL);
	return (fk);
}

void	init_helpu(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_ph)
	{
		table->philos[i] = init_philos(i, table);
		if (!table->philos[i])
			return (ft_free_(&table->philos), ft_perror("init philo"));
		table->forks[i] = init_forks(i, table);
		if (!table->forks[i])
			return (ft_free_(&table->philos),
			ft_free_(&table->forks), ft_perror("init fork"));
		i++;
	}
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
	table->philos = (t_philo **)malloc(sizeof(t_philo) * table->n_ph);
	if (!table->philos)
		return ;
	table->forks = (pthread_mutex_t **)malloc(sizeof(pthread_mutex_t) * table->n_ph);
	if (!table->forks)
		return (free(&table->philos));
	init_helpu(table);
	table->start_time = 0;
}
