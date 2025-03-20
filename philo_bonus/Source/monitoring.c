/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:33:49 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/20 15:27:08 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitoring_thread(void *arg)
{
	t_rules	*rules;

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
		sem_wait(rules->print_sem);
		printf(END_AFTER_N_MEALS, time_passed, rules->nb_meat);
		sem_post(rules->print_sem);
		sem_wait(rules->death_sem);
		rules->stop_simulation = 1;
		sem_post(rules->death_sem);
	}
}

int	philo_died(t_rules *rules, int i)
{
	long	current_time;
	long	time_since_last_meal;

	current_time = get_time_in_ms();
	sem_wait(rules->philos[i].last_meal_sem);
	time_since_last_meal = current_time - rules->philos[i].last_meal_time;
	sem_post(rules->philos[i].last_meal_sem);
	if (time_since_last_meal > rules->time_to_die)
	{
		print_status(rules->philos[i].id, "died", &rules->philos[i]);
		sem_wait(rules->death_sem);
		rules->stop_simulation = 1;
		sem_post(rules->death_sem);
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
		sem_wait(rules->philos[i].nb_meals_sem);
		if (rules->philos[i].nb_meals < rules->nb_meat)
		{
			sem_post(rules->philos[i].nb_meals_sem);
			return (0);
		}
		sem_post(rules->philos[i].nb_meals_sem);
		i++;
	}
	return (1);
}
