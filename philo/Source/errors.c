/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:26:07 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/20 21:31:10 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_exit(const char *message)
{
	printf("%s\n", message);
	exit(1);
}

void	error_malloc_philos(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->nb_philos)
		pthread_mutex_destroy(&rules->forks[i++]);
	pthread_mutex_destroy(&rules->print_mutex);
	pthread_mutex_destroy(&rules->death_mutex);
	free(rules->forks);
	free(rules->philos);
	error_exit(ERROR_MALLOC_PHILOS);
}
