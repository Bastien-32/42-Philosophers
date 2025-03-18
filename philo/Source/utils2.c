/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:38:24 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/18 17:38:59 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(int id, char *status, t_philo *philo)
{
	long	time_passed;

	time_passed = get_time_in_ms() - philo->rules->start_time;
	pthread_mutex_lock(&philo->rules->print_mutex);
	printf("%ld %d %s\n",time_passed, id, status);
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

void	quit_program(t_rules *rules, long time_passed, char *msg)
{
	int j;
	int	i;

	i = 0;
	j = 0;
	pthread_join(rules->monitor_thread, NULL);
	while (j < rules->nb_philos)
		pthread_mutex_destroy(&rules->forks[j++]);
	pthread_mutex_destroy(&rules->print_mutex);
	pthread_mutex_destroy(&rules->death_mutex);
	free(rules->forks);
	free(rules->philos);
	printf(msg, time_passed, rules->nb_meat);
	exit (1);
}