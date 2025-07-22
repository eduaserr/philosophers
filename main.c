/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/23 00:55:06 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/philo.h"

int	simulation(t_table *table)
{
	pthread_t	reaper;
	int			i;

	i = -1;
	table->start_time = get_time();
	//crear threads
	while (++i < table->n_ph)
	{
		pthread_mutex_lock(&table->meal_mutex);
		table->philos[i].last_meal = table->start_time;
		pthread_mutex_unlock(&table->meal_mutex);
		if (pthread_create(&table->philos[i].thread, NULL, ph_routine, &table->philos[i]) != 0)
			return (ft_perror("philosopher thread"), EXIT_FAILURE);
	}
	//create reaper
	//printf("reaper\n");
	if (pthread_create(&reaper, NULL, dh_routine, table) != 0)
		return (ft_perror("reaper thread"), EXIT_FAILURE);
	//esperar reaper
	pthread_join(reaper, NULL);
	i = -1;
	//esperar filosofos
	while (++i < table->n_ph)
		pthread_join(table->philos[i].thread, NULL);
	return (0);
}

int	main(int ac, char **av)
{
	t_table	table;

	if (check_args(ac, av))
		return (1);
	init_table(&table, av);
	if (simulation(&table))
		return (ft_free_all(&table), EXIT_FAILURE);
	//ft_printtable(&table);
	ft_free_all(&table);
	printf("philo :)\n");
	return (0);
}
