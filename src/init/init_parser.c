/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 13:09:20 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/11 13:10:10 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

static int	check_digit(char **str)
{
	int	i;
	int	j;

	i = 1;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			if (!ft_isdigit(str[i][j]))
				return (EXIT_FAILURE);
			j++;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	check_args(int ac, char **av)
{
	int	nb;

	ac -= 1;
	if (ac < 4 || ac > 5)
		return (ft_perror("ia"), EXIT_FAILURE);
	if (check_digit(av))
		return (ft_perror("od"), EXIT_FAILURE);
	nb = ft_atoi(av[1]);
	if (nb < 1 || nb > 200)
		return (ft_perror("if"), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
