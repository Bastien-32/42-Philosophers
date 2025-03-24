/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:33:49 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/24 14:27:47 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitoring_thread(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		usleep(1000);
		check_philosophers(philo);
	}
	return (NULL);
}

int	check_philosophers(t_philo *philo)
{
	if (philo_died(philo))
		exit (1);
	if (philo_full(philo))
		exit (0);
	return (0);
}

int	philo_died(t_philo *philo)
{
	long	current_time;
	long	time_since_last_meal;

	current_time = get_time_in_ms();
	sem_wait(philo->last_meal_sem);
	time_since_last_meal = current_time - philo->last_meal_time;
	if (time_since_last_meal > philo->rules->time_to_die)
	{
		//print_status(philo->id, "died", philo);
		sem_wait(philo->rules->print_sem);
		return (1);
	}
	sem_post(philo->last_meal_sem);
	return (0);
}

int	philo_full(t_philo *philo)
{

	if (philo->rules->nb_meat <= 0)
		return (0);
	sem_wait(philo->nb_meals_sem);
	if (philo->nb_meals < philo->rules->nb_meat)
	{
		sem_post(philo->nb_meals_sem);
		return (0);
	}
	sem_post(philo->nb_meals_sem);
	return (1);
}

