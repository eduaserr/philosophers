/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 20:43:07 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/11 03:29:00 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

int	ft_isdigit(int a)
{
	return (a >= '0' && a <= '9');
}

int	ft_atoi(const char *str)
{
	int	i;
	int	number;

	i = 0;
	number = 0;
	while (ft_isdigit(str[i]))
	{
		number *= 10;
		number += str[i] - 48;
		i++;
	}
	return (number);
}

static int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	ft_perror(char *msg)
{
	if (ft_strcmp("ia", msg) == 0)
		printf(E_MSG, "Invalid number of arguments");
	else if (ft_strcmp("od", msg) == 0)
		printf(E_MSG, "Only digits allowed");
	else if (ft_strcmp("if", msg) == 0)
		printf(E_MSG, "Insuficients forks!");
}
