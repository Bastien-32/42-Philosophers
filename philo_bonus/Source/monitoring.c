/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:33:49 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/22 17:57:48 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitoring_thread(void *arg)
{
	t_rules	*rules;

	rules = (t_rules *)arg;
	while (1)
	{
		usleep(1000);
		if(check_philosophers(rules))
			break ;
	}
	return (NULL);
}

int	check_philosophers(t_rules *rules)
{
	int		i;
	long	time_passed;

	if (philo_died(rules))
		return (1);

	if (all_philos_full(rules))
	{
		time_passed = get_time_in_ms() - rules->start_time;
		sem_wait(rules->print_sem);
		printf(END_AFTER_N_MEALS, time_passed, rules->nb_meat);
		sem_post(rules->print_sem);
		return (1);
	}
	return (0);
}

/* int	check_philosophers(t_rules *rules)
{
	int		i;
	long	time_passed;

	i = 0;
	while (i < rules->nb_philos)
	{
		if (philo_died(rules, i))
			return (1);
		i++;
	}
	if (all_philos_full(rules))
	{
		time_passed = get_time_in_ms() - rules->start_time;
		sem_wait(rules->print_sem);
		printf(END_AFTER_N_MEALS, time_passed, rules->nb_meat);
		sem_post(rules->print_sem);
		kill_all_philos(rules);
		return (1);
	}
	return (0);
} */


void	*monitor_life(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		usleep(1000);
		sem_wait(philo->last_meal_sem);
		if (get_time_in_ms() - philo->last_meal_time > \
													philo->rules->time_to_die)
		{
			sem_post(philo->last_meal_sem);
			print_status(philo->id, "died", philo);
			exit(1);
		}
		sem_post(philo->last_meal_sem);
		sem_wait(philo->nb_meals_sem);
		if (philo->nb_meals >= philo->rules->nb_meat && philo->rules->nb_meat > 0)
		{
			sem_post(philo->nb_meals_sem);
			sem_wait(rules->print_sem);
			printf(END_AFTER_N_MEALS, time_passed, rules->nb_meat);
			sem_post(rules->print_sem);
			exit(0);
		}
		sem_post(philo->nb_meals_sem);
	}
	return (NULL);
}

int	philo_died(t_philo *philo, int i)
{
	long	current_time;
	long	time_since_last_meal;

	current_time = get_time_in_ms();
	sem_wait(philo->last_meal_sem);
	time_since_last_meal = current_time - philo->last_meal_time;
	sem_post(philo->last_meal_sem);
	sem_wait(philo->last_meal_sem);
	if (time_since_last_meal > philo->rules->time_to_die)
	{
		sem_post(philo->last_meal_sem);
		print_status(philo->id, "died", philo);
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

void	kill_all_philos(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->nb_philos)
	{
		kill(rules->philos[i].pid, SIGTERM);
		i++;
	}
}
