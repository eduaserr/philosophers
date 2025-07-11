/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 13:05:14 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/11 13:05:46 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

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
