/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 18:45:46 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/17 19:50:46 by eduaserr         ###   ########.fr       */
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
		if (eat(ph) || check_someone_died(ph->table))
			break ;
		if (think(ph) || check_someone_died(ph->table))
			break ;
		if (ft_sleep(ph) || check_someone_died(ph->table))
			break ;
	}
	return (NULL);
}

void *dh_routine(void *arg)
{
	t_table	*table;
	int		i;
	int		all_done;

	table = (t_table *)arg;
	while (1)
	{
		i = -1;
		while (++i < table->n_ph)
		{
			if (check_death(&table->philos[i]))
				return (someone_died(table), print_msg(table->philos, "died"), NULL);
		}
		if (table->n_meals != -1)  // Si hay límite de comidas
		{
			all_done = 1;
			i = -1;
			while (++i < table->n_ph)
			{
				pthread_mutex_lock(&table->meal_mutex);
				if (table->philos[i].meals < table->n_meals)  // Usa check_meals() o acceso directo
				{
					all_done = 0;
					pthread_mutex_unlock(&table->meal_mutex);
					break ;
				}
				pthread_mutex_unlock(&table->meal_mutex);
			}
			if (all_done)
				return (someone_died(table), NULL);
		}
		usleep(1000);  // Verificar cada 1ms
	}
}
