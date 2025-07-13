/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:08:03 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/13 23:10:30 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

void ft_printfilo_single(t_philo *ph)
{
	printf("  Filósofo %d:\n", ph->id);
	printf("	Comidas: %d\n", ph->meals);
	printf("	Última comida: %ld ms\n", ph->last_meal);
	printf("	Tenedor izq: %p\n", (void *)ph->l_fork);
	printf("	Tenedor der: %p\n", (void *)ph->r_fork);
	printf("	Thread ID: %lu\n", ph->thread);
	printf("\n");
}

void ft_printfilo_all(t_table *table)
{
	int i;

	printf("=== FILÓSOFOS ===\n");
	i = 0;
	while (i < table->n_ph)
	{
		ft_printfilo_single(&table->philos[i]);
		i++;
	}
}

void ft_printforks(t_table *table)
{
	int i;

	printf("=== TENEDORES ===\n");
	i = 0;
	while (i < table->n_ph)
	{
		printf("  Tenedor %d: %p\n", i, (void *)&table->forks[i]);
		i++;
	}
	printf("\n");
}

void ft_print_ids(t_table *table)
{
	int i;
		
	printf("IDs de filósofos: ");
	i = 0;
	while (i < table->n_ph)
	{
		printf("%d ", table->philos[i].id);
		i++;
	}
	printf("\n");
}

void ft_debug_initialization(t_table *table)
{
	int i;

	printf("\n=== DEBUG INICIALIZACIÓN ===\n");
	printf("n_ph: %d\n", table->n_ph);
	printf("philos array: %p\n", (void *)table->philos);
	printf("forks array: %p\n", (void *)table->forks);

	if (table->philos)
	{
		printf("Filósofos inicializados:\n");
		i = 0;
		while (i < table->n_ph)
		{
			printf("  philo[%d]: id=%d, l_fork=%p, r_fork=%p\n", 
				   i, table->philos[i].id, 
				   (void *)table->philos[i].l_fork,
				   (void *)table->philos[i].r_fork);
			i++;
		}
	}
	else
	{
		printf("❌ Array de filósofos es NULL\n");
	}
	if (table->forks)
	{
		printf("Tenedores en direcciones:\n");
		i = 0;
		while (i < table->n_ph)
		{
			printf("  fork[%d]: %p\n", i, (void *)&table->forks[i]);
			i++;
		}
	}
	else
	{
		printf("❌ Array de tenedores es NULL\n");
	}
	printf("==========================\n\n");
}

void ft_printtable(t_table *table)
{
	printf("\n========== ESTADO DE LA MESA ==========\n");
	printf("Número de filósofos: %d\n", table->n_ph);
	printf("Tiempo para morir: %ld ms\n", table->time_to_die);
	printf("Tiempo para comer: %ld ms\n", table->time_to_eat);
	printf("Tiempo para dormir: %ld ms\n", table->time_to_sleep);
	printf("Comidas necesarias: %d\n", table->n_meals);
	printf("Tiempo de inicio: %ld ms\n", table->start_time);
	printf("=====================================\n\n");

	ft_printforks(table);
	ft_printfilo_all(table);
}
