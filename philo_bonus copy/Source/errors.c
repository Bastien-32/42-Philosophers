/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:26:07 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/20 21:30:18 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	error_exit(const char *message)
{
	printf("%s\n", message);
	exit(1);
}

void	error_malloc_philos(t_rules *rules)
{
	sem_close(rules->forks);
	sem_unlink("/forks");
	sem_close(rules->print_sem);
	sem_unlink("/print");
	sem_close(rules->death_sem);
	sem_unlink("/death");
	free(rules->philos);
	error_exit(ERROR_MALLOC_PHILOS);
}
