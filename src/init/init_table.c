/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 01:18:24 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/13 03:29:39 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

/* void	ft_free_philos(t_philo ***ph)
{
	free(**ph);
} */

void ft_printfilo_single(t_philo *ph)
{
    printf("  Filósofo %d:\n", ph->id);
    printf("    Comidas: %d\n", ph->meals);
    printf("    Última comida: %ld ms\n", ph->last_meal);
    printf("    Tenedor izq: %p\n", (void *)ph->l_fork);
    printf("    Tenedor der: %p\n", (void *)ph->r_fork);
    printf("    Thread ID: %lu\n", ph->thread);
    printf("\n");
}

// Imprimir TODOS los filósofos
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

// Imprimir todos los tenedores
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

// Imprimir información completa de la mesa
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

// Función simple para debug - solo IDs
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

// Función para verificar inicialización
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

void ft_free_forks(t_table *table)
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

void	ft_free_all(t_table **table)
{
	ft_free_ph(*table);
	ft_free_forks(*table);
	*table = NULL;
}

void	init_philos(int id, t_table *table, t_philo *ph)
{
	ph->id = id;
	ph->meals = 0;
	ph->last_meal = 0;
	ph->l_fork = &table->forks[id];
	ph->r_fork = &table->forks[(id + 1) % table->n_ph];
	ph->thread = 0;
	ph->table = (struct t_table *)table;
}

int	init_forks(int id, t_table *table)
{
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
			return (ft_perror("init philo"), 1);
		if (init_forks(i, table) == 1)
			return (ft_perror("init fork"), 1);
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
		return (ft_free_all(&table), ft_perror("mutex init"));
	table->start_time = 0;
	ft_printtable(table);
	ft_free_all(&table);
}

//faltan funciones para liberar philo y fork.