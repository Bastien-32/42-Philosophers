/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:33:49 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/19 17:35:57 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitoring_thread(void *arg)
{
	t_rules *rules;

	rules = (t_rules *)arg;
	while (!rules->stop_simulation)
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
		if (philo_died(rules, i))
			return ;
		i++;
	}	
	if (all_philos_full(rules))
	{
		time_passed = get_time_in_ms() - rules->start_time;
		pthread_mutex_lock(&rules->print_mutex);
		printf(END_AFTER_N_MEALS, time_passed, rules->nb_meat);
		pthread_mutex_unlock(&rules->print_mutex);
		pthread_mutex_lock(&rules->death_mutex);
		rules->stop_simulation = 1;
		pthread_mutex_unlock(&rules->death_mutex);
	}
}

int	philo_died(t_rules *rules, int i)
{
	long	current_time;
	long	time_since_last_meal;

	current_time = get_time_in_ms();
	pthread_mutex_lock(&rules->philos[i].last_meal_mutex);
	time_since_last_meal = current_time - rules->philos[i].last_meal_time;
	pthread_mutex_unlock(&rules->philos[i].last_meal_mutex);
	if (time_since_last_meal > rules->time_to_die)
	{
		print_status(rules->philos[i].id, "died", &rules->philos[i]);
		pthread_mutex_lock(&rules->death_mutex);
		rules->stop_simulation = 1;
		pthread_mutex_unlock(&rules->death_mutex);
		return (1);
	}
	return (0);
}

int	all_philos_full(t_rules *rules)
{
	int	i;

	i = 0;
	if (rules->nb_meat <= 0)
		return (0);
	while (i < rules->nb_philos)
	{
		pthread_mutex_lock(&rules->philos[i].nb_meals_mutex);
		if (rules->philos[i].nb_meals < rules->nb_meat)
		{
			pthread_mutex_unlock(&rules->philos[i].nb_meals_mutex);
			return (0);
		}
		pthread_mutex_unlock(&rules->philos[i].nb_meals_mutex);
		i++;
	}
	return (1);
}

