/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 01:18:24 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/12 21:09:29 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

/* void	ft_free_philos(t_philo ***ph)
{
	free(**ph);
} */

void	init_philos(int id, t_table *table, t_philo *ph)
{
	ph->id = id;
	ph->meals = 0;
	ph->last_meal = 0;
	ph->l_fork = &table->forks[id];
	ph->r_fork = &table->forks[(id + 1) % table->n_ph];
	ph->thread = 0;
	ph->table = table;
}

int	init_forks(int id, t_table *table)
{
	int n_ph;

	n_ph = table->n_ph;
	if (pthread_mutex_init(&table->forks[id], NULL) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	init_helpu(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_ph)
	{
		init_philos(i, table, &table->philos[i]);
		if (!table->philos)
			return (ft_free_ph(&table->philos), ft_perror("init philo"), 1);
		init_forks(i, table);
		if (!table->forks)
			return (ft_free_ph(&table->philos),
			ft_free_fk(&table->forks), ft_perror("init fork"), 1);
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
	table->philos = (t_philo *)malloc(sizeof(t_philo) * table->n_ph);
	if (!table->philos)
		return ;
	table->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * table->n_ph);
	if (!table->forks)
		return (free(table->forks));
	init_helpu(table);
	table->start_time = 0;

}

//faltan funciones para liberar philo y fork.