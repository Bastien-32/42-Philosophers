/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:33:49 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/18 17:54:20 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitoring_thread(void *arg)
{
	t_rules *rules = (t_rules *)arg;

	while (!rules->simulation_end)
	{
		check_philosophers(rules);
		usleep(100);
	}
	return (NULL);
}

void	check_philosophers(t_rules *rules)
{
	int		i;
	long	time_passed;

	i = 0;
	while (i < rules->nb_philos)
	{
		if (philo_has_died(rules, i))
			return ;
		i++;
	}
	if (all_philos_full(rules))
	{
		pthread_mutex_lock(&rules->death_mutex);
		rules->stop_simulation = 1;
		pthread_mutex_unlock(&rules->death_mutex);
		time_passed = get_time_in_ms() - rules->start_time;
		quit_program(rules, time_passed, END_AFTER_N_MEALS);
	}
}


int	philo_has_died(t_rules *rules, int i)
{
	long	current_time;
	long	time_since_last_meal;

	current_time = get_time_in_ms();
	pthread_mutex_lock(&rules->philos[i].meal_mutex);
	time_since_last_meal = current_time - rules->philos[i].last_meal_time;
	if (time_since_last_meal > rules->time_to_die)
		philo_die(rules, i);
	pthread_mutex_unlock(&rules->philos[i].meal_mutex);
	return (0);
}

void	philo_die(t_rules *rules, int i)
{
	print_status(rules->philos[i].id, "died", &rules->philos[i]);
	pthread_mutex_lock(&rules->death_mutex);
	rules->stop_simulation = 1;
	pthread_mutex_unlock(&rules->death_mutex);
	pthread_mutex_unlock(&rules->philos[i].meal_mutex);
}

int	all_philos_full(t_rules *rules)
{
	int	i;
	int	full_philos;

	i = 0;
	full_philos = 0;
	if (rules->nb_meat <= 0)
		return (0);
	while (i < rules->nb_philos)
	{
		pthread_mutex_lock(&rules->philos[i].meal_mutex);
		if (rules->philos[i].nb_meals < rules->nb_meat)
		{
			pthread_mutex_unlock(&rules->philos[i].meal_mutex);
			return (0);
		}
		pthread_mutex_unlock(&rules->philos[i].meal_mutex);
		i++;
	}
	return (1);
}

