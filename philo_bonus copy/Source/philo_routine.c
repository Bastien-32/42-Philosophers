/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:12:40 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/20 20:48:53 by badal-la         ###   ########.fr       */
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
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(200);
	while (!stop_simu(philo))
	{
		philo_is_eating(philo);
		if (stop_simu(philo))
			break ;
		print_status(philo->id, "is sleeping", philo);
		if (stop_simu(philo))
			break ;
		ft_usleep(philo->rules->time_to_sleep);
		if (stop_simu(philo))
			break ;
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
	print_status(philo->id, "is eating", philo);
	sem_wait(philo->last_meal_sem);
	philo->last_meal_time = get_time_in_ms();
	sem_post(philo->last_meal_sem);
	ft_usleep(philo->rules->time_to_eat);
	sem_post(philo->rules->forks);
	sem_post(philo->rules->forks);
	sem_wait(philo->nb_meals_sem);
	philo->nb_meals++;
	sem_post(philo->nb_meals_sem);
}
