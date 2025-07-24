/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:08:03 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/24 20:54:58 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

/*int	print_msg2(t_philo *ph, char *msg)
{
	if (pthread_mutex_lock(&ph->table->print_mutex) != 0)
		return (1);
	if (ft_strcmp("l_fork", msg) == 0 || ft_strcmp("r_fork", msg) == 0)
		printf("[%lu]ms. Philo[%i] has taken %s\n",
			get_timestamp(ph->table), ph->id + 1);
	else if ((ft_strcmp("eating", msg) == 0
		|| ft_strcmp("sleeping", msg) == 0) || ft_strcmp("thinking", msg) == 0)
		printf("[%lu]ms. Philo[%i] is %s\n",
		get_timestamp(ph->table), ph->id + 1, msg);
	else
		printf("[%lu]ms. Philo[%i] %s\n",
		get_timestamp(ph->table), ph->id + 1, msg);
	if (pthread_mutex_unlock(&ph->table->print_mutex) != 0)
		return (1);
	return (0);
}*/

//msg_eat, msg_sleep, msg_think ...
int	print_msg(t_philo *ph, char *msg)
{
	if (check_someone_died(ph->table))
		return (1);
	if (pthread_mutex_lock(&ph->table->print_mutex) != 0)
		return (1);
	if (ft_strcmp("l_fork", msg) == 0 || ft_strcmp("r_fork", msg) == 0)
		printf("%lu %i has taken a fork\n",
			get_timestamp(ph->table), ph->id + 1);
	else if ((ft_strcmp("eating", msg) == 0
			|| ft_strcmp("sleeping", msg) == 0)
		|| ft_strcmp("thinking", msg) == 0)
		printf("%lu %i is %s\n", get_timestamp(ph->table), ph->id + 1, msg);
	else
		printf("%lu %i %s\n", get_timestamp(ph->table), ph->id + 1, msg);
	if (pthread_mutex_unlock(&ph->table->print_mutex) != 0)
		return (1);
	return (0);
}
