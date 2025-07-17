/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/17 15:56:05 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/philo.h"

int	check_someone_died(t_table *table)
{
	int	died;

	died = 0;
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

	// Si pasó más tiempo del permitido, está muerto
	if (current_time - last_meal > ph->table->time_to_die)
		return (1);  // Muerto

	return (0);  // Vivo
}

int	check_meals(t_philo *ph)
{
	int	meals;

	// Si n_meals es -1, no hay límite
	if (ph->table->n_meals == -1)
		return (0);  // Continuar comiendo
	pthread_mutex_lock(&ph->table->meal_mutex);
	meals = ph->meals;
	pthread_mutex_unlock(&ph->table->meal_mutex);
	// Si ya comió suficiente, terminar
	if (meals >= ph->table->n_meals)
		return (1);  // Terminado
	return (0);  // Continuar
}

void	*ph_routine(void *arg)
{
	t_philo *ph;

	ph = (t_philo *)arg;
	// Caso especial: 1 filósofo
	if (ph->table->n_ph == 1)
	{
		print_msg(ph, "has taken a fork");
		usleep(ph->table->time_to_die * 1000);
		return (NULL);
	}
	// Estrategia anti-deadlock: impares empiezan con delay
	if (ph->id % 2 == 1)
		usleep(ph->table->time_to_eat * 500);  // Medio tiempo de comer
	pthread_mutex_lock(&ph->table->meal_mutex);
	ph->last_meal = ph->table->start_time;
	pthread_mutex_unlock(&ph->table->meal_mutex);
	while (!check_someone_died(ph->table)) // check_death , check_meals
	{
		if (eat(ph) == 1)
			break ;
		if (think(ph))
			break ;
		if (ft_sleep(ph))
			break ;
	}
	return (NULL);
}


void *dh_routine(void *arg)
{
	t_table *table = (t_table *)arg;
	int i;
		
	while (1)
	{
		i = 0;
		while (i < table->n_ph)
		{
			if (check_death(&table->philos[i]))  // 🔄 Reutilizar tu función
			{
				// 🔒 Marcar muerte globalmente
				pthread_mutex_lock(&table->death_mutex);
				table->someone_died = 1;
				pthread_mutex_unlock(&table->death_mutex);
				
				// 📢 Anunciar muerte
				pthread_mutex_lock(&table->print_mutex);
				printf("[%ld]ms %d died\n", get_timestamp(table), table->philos[i].id);
				pthread_mutex_unlock(&table->print_mutex);
				
				return (NULL);
			}
			i++;
		}
		if (table->n_meals != -1)  // Si hay límite de comidas
		{
			int all_done = 1;
			i = 0;
			while (i < table->n_ph)
			{
				if (table->philos[i].meals < table->n_meals)  // Usa check_meals() o acceso directo
				{
					all_done = 0;
					break;
				}
				i++;
			}
			if (all_done)
			{
				pthread_mutex_lock(&table->death_mutex);
				table->someone_died = 1;  // Terminar simulación
				pthread_mutex_unlock(&table->death_mutex);
				return (NULL);
			}
		}
		
		usleep(1000);  // Verificar cada 1ms
	}
}

int	simulation(t_table *table)
{
	pthread_t	reaper;
	int			i;

	i = -1;
	(void)i;
	table->start_time = get_time();
	//crear threads
	while (++i < table->n_ph)
	{
		table->philos[i].last_meal = table->start_time;
		if (pthread_create(&table->philos[i].thread, NULL, ph_routine, &table->philos[i]) != 0)
			return (ft_perror("philosopher thread"), EXIT_FAILURE);
	}
	//create reaper
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


/*
	 mutex lock
	COGER FORKS
	 mutex unlock

	mutex lock
	- PRINTF fork obtenido (left or right)
	mutex unlock

	mutex lock
	- comer
	mutex unlock

	mutex lock
	- PRINTF comer
	mutex unlock

	 mutex lock
	soltar forks
	 mutex unlock

	mutex lock
	- PRINTF fork soltado (left or right)
	mutex unlock

	mutex lock
	- pensar
	mutex unlock

	mutex lock
	- PRINTF pensar
	mutex unlock

	mutex lock
	- dormir
	mutex unlock

	mutex lock
	- PRINTF dormir
	mutex unlock
*/
