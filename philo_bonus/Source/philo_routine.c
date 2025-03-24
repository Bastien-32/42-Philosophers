/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:12:40 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/24 09:56:39 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	stop_simu(t_philo *philo)
{
	sem_wait(philo->rules->death_sem);
	if (philo->rules->stop_simulation)
	{
		sem_post(philo->rules->death_sem);
		return (1);
	}
	sem_post(philo->rules->death_sem);
	return (0);
}

void	*philos_routine(void *arg)
{
	t_philo		*philo;
	pthread_t	monitor;

	philo = (t_philo *)arg;
	sem_wait(philo->last_meal_sem);
	philo->last_meal_time = get_time_in_ms();
	sem_post(philo->last_meal_sem);
	if(pthread_create(&monitor, NULL, monitoring_thread, philo) != 0)
		exit(1);
	pthread_detach(monitor);
	if (philo->id % 2 == 0)
		ft_usleep(200);
	while (1)
	{
		philo_is_eating(philo);
		print_status(philo->id, "is sleeping", philo);
		ft_usleep(philo->rules->time_to_sleep);
		print_status(philo->id, "is thinking", philo);
	}
	return (NULL);
}

void	philo_is_eating(t_philo *philo)
{
	sem_wait(philo->rules->forks);
	print_status(philo->id, HAS_TAKEN_FORK, philo);
	sem_wait(philo->rules->forks);
	print_status(philo->id, HAS_TAKEN_FORK, philo);
	sem_wait(philo->last_meal_sem);
	philo->last_meal_time = get_time_in_ms();
	sem_post(philo->last_meal_sem);
	print_status(philo->id, "is eating", philo);
	ft_usleep(philo->rules->time_to_eat);
	sem_post(philo->rules->forks);
	sem_post(philo->rules->forks);
	sem_wait(philo->nb_meals_sem);
	philo->nb_meals++;
	sem_post(philo->nb_meals_sem);
}

