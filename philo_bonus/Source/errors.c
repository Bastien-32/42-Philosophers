/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:26:07 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/20 15:20:53 by badal-la         ###   ########.fr       */
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
	int	i;

	i = 0;
	sem_close(rules->forks);
	sem_unlink("/forks");
	sem_close(rules->print_sem);
	sem_unlink("/print");
	sem_close(rules->death_sem);
	sem_unlink("/death");
	free(rules->philos);
	error_exit(ERROR_MALLOC_PHILOS);
}

/*void	error_create_monitor(t_rules *rules)
{
	int	i;
	int j;

	i = 0;
	j = 0;
	pthread_join(rules->monitor_thread, NULL);
	while (j < rules->nb_philos)
		pthread_mutex_destroy(&rules->forks[j++]);
	pthread_mutex_destroy(&rules->print_mutex);
	pthread_mutex_destroy(&rules->death_mutex);
	free(rules->forks);
	free(rules->philos);
	error_exit(ERROR_MONITOR);
}

void	error_create_threads(t_rules *rules, int i)
{
	int	j;

	j = 0;
	rules->stop_simulation = 1;
	while (j < i)
		pthread_join(rules->philos[j++].thread, NULL);
	pthread_join(rules->monitor_thread, NULL);
	j = 0;
	while (j < rules->nb_philos)
		pthread_mutex_destroy(&rules->forks[j++]);
	pthread_mutex_destroy(&rules->print_mutex);
	pthread_mutex_destroy(&rules->death_mutex);
	free(rules->forks);
	free(rules->philos);
	printf(ERROR_THREAD, rules->philos[i].id);
	exit (1);
}*/
