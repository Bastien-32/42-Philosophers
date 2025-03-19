/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:12:40 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/19 14:22:33 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*void	*philos_routine(void *arg)
{
	t_philo	*philo;
	philo = (t_philo *)arg;

	if (philo->id == philo->rules->nb_philos && philo->rules->nb_philos % 2 != 0)
		usleep(100);
	while (1)
	{		
		if (philo->rules->stop_simulation)
			break ;
		philo_is_eating(philo);
		if (philo->rules->stop_simulation)
			break ;
		print_status(philo->id, "is sleeping", philo);
		if (philo->rules->stop_simulation)
			break ;
		ft_usleep(philo->rules->time_to_sleep);
		if (philo->rules->stop_simulation)
			break ;
		print_status(philo->id, "is thinking", philo);
		if (philo->rules->stop_simulation)
			break ;
	}
	return (NULL);
}*/

void	*philos_routine(void *arg)
{
	t_philo	*philo;
	philo = (t_philo *)arg;

	if (philo->id == philo->rules->nb_philos && philo->rules->nb_philos % 2 != 0)
		usleep(100);
	while (!philo->rules->stop_simulation)
	{
		philo_is_eating(philo);
		if (philo->rules->stop_simulation)
			break ;
		print_status(philo->id, "is sleeping", philo);
		if (philo->rules->stop_simulation)
			break ;
		ft_usleep(philo->rules->time_to_sleep);
		if (philo->rules->stop_simulation)
			break ;
		print_status(philo->id, "is thinking", philo);
	}
	return (NULL);
}

void	philo_is_eating(t_philo *philo)
{
	take_forks(philo);
	if (philo->rules->stop_simulation)
		return ;
	print_status(philo->id, "is eating", philo);
	philo->last_meal_time = get_time_in_ms();
	ft_usleep(philo->rules->time_to_eat);
	release_forks(philo);
	pthread_mutex_lock(&philo->nb_meals_mutex);
	philo->nb_meals++;
	pthread_mutex_unlock(&philo->nb_meals_mutex);
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->f_right);
		print_status(philo->id, HAS_TAKEN_FORK, philo);
		if (philo->rules->stop_simulation)
		{
			pthread_mutex_unlock(philo->f_right);
			return ;
		}
		pthread_mutex_lock(philo->f_left);
		print_status(philo->id, HAS_TAKEN_FORK, philo);
		if (philo->rules->stop_simulation)
		{
			pthread_mutex_unlock(philo->f_right);
			pthread_mutex_unlock(philo->f_left);
			return ;
		}
	}
	else
	{
		pthread_mutex_lock(philo->f_left);
		print_status(philo->id, HAS_TAKEN_FORK, philo);
		if (philo->rules->stop_simulation)
		{
			pthread_mutex_unlock(philo->f_left);
			return ;
		}
		pthread_mutex_lock(philo->f_right);
		print_status(philo->id, HAS_TAKEN_FORK, philo);
		if (philo->rules->stop_simulation)
		{
			pthread_mutex_unlock(philo->f_right);
			pthread_mutex_unlock(philo->f_left);
			return ;
		}
	}
}

void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->f_right);
	pthread_mutex_unlock(philo->f_left);
}