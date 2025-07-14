/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/14 00:47:44 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/philo.h"

long get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);  // tiempo en ms
}
/*
long get_timestamp(t_table *table)
{
    return (get_time() - table->start_time);
}
*/

int	simulation(t_table *table)
{
	//pthread_t	reaper;
	int			i;

	i = -1;
	(void)i;
	table->start_time = get_time();
	//crear threads
	/*while (++i < table->n_ph)
	{
		table->philos[i].last_meal = table->start_time;
		if (pthread_create(&table->philos[i].thread, NULL, function routine, &table->philos[i]) != 0)
			return (ft_perror("philosopher thread"), EXIT_FAILURE);
	}
	//create reaper
	if (pthread_create(&reaper, NULL, function routine, table) != 0)
		return (ft_perror("reaper thread"), EXIT_FAILURE);
	//esperar reaper
	pthread_join(reaper, NULL);
	i = -1;
	//esperar filosofos
	while (++i < table->n_ph)
		pthread_join(table->philos[i].thread, NULL);*/
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
	ft_printtable(&table);
	ft_free_all(&table);
	printf("philo :)\n");
	return (0);
}
