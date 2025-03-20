/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:38:24 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/20 10:26:13 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(int id, char *status, t_philo *philo)
{
	long	time_passed;

	time_passed = get_time_in_ms() - philo->rules->start_time;
	pthread_mutex_lock(&philo->rules->death_mutex);
	if (!philo->rules->stop_simulation)
	{
		pthread_mutex_lock(&philo->rules->print_mutex);
		printf("%ld %d %s\n", time_passed, id, status);
		pthread_mutex_unlock(&philo->rules->print_mutex);
		pthread_mutex_unlock(&philo->rules->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->rules->death_mutex);
}

void	quit_program(t_rules *rules)
{
	int	j;

	j = 0;
	usleep(200);
	pthread_join(rules->monitor_thread, NULL);
	while (j < rules->nb_philos)
		pthread_join(rules->philos[j++].thread, NULL);
	j = 0;
	while (j < rules->nb_philos)
	{
		pthread_mutex_destroy(&rules->forks[j]);
		pthread_mutex_destroy(&rules->philos[j].last_meal_mutex);
		pthread_mutex_destroy(&rules->philos[j].nb_meals_mutex);
		j++;
	}
	pthread_mutex_destroy(&rules->print_mutex);
	pthread_mutex_destroy(&rules->death_mutex);
	free(rules->forks);
	free(rules->philos);
}
