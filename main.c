/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/10 21:17:41 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/philo.h"

int	check_digit(char *str) // cheqear todos los argumentos disponibles
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!is_digit(str[i]))
			return ();
		i++;
	}
}

int	check_args(int ac, char **av)
{
	(void)av;
	ac -= 1;
	if (ac < 4 || ac > 5)
		return (1);
	if (!is_digit())
	return (0);
}

int	main(int ac, char **av)
{
	t_table	table;

	if (check_args(ac, av))
		return (ft_perror(&table, "ia", 0), 1); //usar code o usar mensaje, un invalid args para todo, o un mensaje por cada error
	//init_table();
	printf("philo :)\n");
	return (0);
}
