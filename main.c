/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/17 03:01:30 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/philo.h"

long get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);  // tiempo en ms
}

long get_timestamp(t_table *table)
{
	return (get_time() - table->start_time);
}

int check_death(t_philo *ph)
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

int check_meals(t_philo *ph)
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

//msg_eat, msg_sleep, msg_think ...
int	print_msg(t_philo *ph, char *msg)
{
	if (pthread_mutex_lock(&ph->table->print_mutex) != 0)
		return (1);
	if (ft_strcmp("l_fork", msg) == 0 || ft_strcmp("r_fork", msg) == 0)
		printf("[%lu]ms %i has taken %s\n", get_timestamp(ph->table), ph->id, msg);
	else if (ft_strcmp("eating", msg) == 0)
		printf("[%lu]ms %i is %s\n", get_timestamp(ph->table), ph->id, msg);
	else
		printf("[%lu]ms %i is %s\n", get_timestamp(ph->table), ph->id, msg);
	if (pthread_mutex_unlock(&ph->table->print_mutex) != 0)
		return (1);
	return (0);
}

static int	get_forks(t_philo *ph)
{
	if (ph->id % 2 == 0)
	{
		if (pthread_mutex_lock(ph->l_fork) != 0)
			return (1);
		if (print_msg(ph, "l_fork") != 0)
			return (1);
		if (pthread_mutex_lock(ph->r_fork) != 0)
			return (1);
		if (print_msg(ph, "r_fork") != 0)
			return (1);
	}
	else
	{
		if (pthread_mutex_lock(ph->r_fork) != 0)
			return (1);
		if (print_msg(ph, "r_fork") != 0)
			return (1);
		if (pthread_mutex_lock(ph->l_fork) != 0)
			return (1);
		if (print_msg(ph, "l_fork") != 0)
			return (1);
	}
	return (0);
}

int	eat(t_philo *ph)
{
	if (get_forks(ph) == 1)
		return (1);
	if (print_msg(ph, "eating"))
		return (1);
	if (pthread_mutex_lock(&ph->table->meal_mutex))
		return (1);
	ph->last_meal = get_time();
	ph->meals++;
	if (pthread_mutex_unlock(&ph->table->meal_mutex))
		return (1);
	usleep(ph->table->time_to_eat * 1000);
	if (pthread_mutex_unlock(ph->l_fork))
		return (1);
	if (pthread_mutex_unlock(ph->r_fork))
		return (1);
	return (0);
}

int	think(t_philo *ph)
{
	(void)ph;
	if (print_msg(ph, "thinking") != 0)
		return (1);
	return (0);
}

int	ft_sleep(t_philo *ph)
{
	(void)ph;
	if (print_msg(ph, "sleeping") != 0)
		return (1);
	usleep(ph->table->time_to_sleep * 1000);
	return (0);
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
	while (!check_death(ph) && !check_meals(ph)) // check_death , check_meals
	{
		if (eat(ph) == 1)
			break ;
		if (!check_death(ph) || !check_meals(ph))
			break ;
		if (think(ph))
			break ;
		if (!check_death(ph) || !check_meals(ph))
			break ;
		if (ft_sleep(ph))
			break ;
	}
	return (NULL);
}


/*void *dh_routine(void *arg)
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
                printf("[%ld]ms %d died\n", get_timestamp(table), table->philos[i].id + 1);
                pthread_mutex_unlock(&table->print_mutex);
                
                return (NULL);
            }
            i++;
        }
        
        usleep(1000);  // Verificar cada 1ms
    }
}*/


int	simulation(t_table *table)
{
	//pthread_t	reaper;
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
	//if (pthread_create(&reaper, NULL, dh_routine, table) != 0)
		//return (ft_perror("reaper thread"), EXIT_FAILURE);
	//esperar reaper
	//pthread_join(reaper, NULL);
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
