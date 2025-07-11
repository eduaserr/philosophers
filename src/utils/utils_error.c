/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 13:08:13 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/11 13:19:08 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/philo.h"

void	ft_perror(char *msg)
{
	if (ft_strcmp("ia", msg) == 0)
		printf(E_MSG, "Invalid number of arguments");
	else if (ft_strcmp("od", msg) == 0)
		printf(E_MSG, "Only digits allowed");
	else if (ft_strcmp("if", msg) == 0)
		printf(E_MSG, "Insuficients forks!");
}
