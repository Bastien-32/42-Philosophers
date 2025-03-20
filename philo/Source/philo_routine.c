/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:12:40 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/20 10:05:31 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int	stop_simu(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->death_mutex);
	if (philo->rules->stop_simulation)
	{
		pthread_mutex_unlock(&philo->rules->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->rules->death_mutex);
	return (0);
}

void	*philos_routine(void *arg)
{
	t_philo	*philo;
	philo = (t_philo *)arg;

	if (philo->id == philo->rules->nb_philos && philo->rules->nb_philos % 2 != 0)
		usleep(100);
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
	if (philo->id % 2 == 0)
		even_take_forks(philo);
	else
		odd_take_forks(philo);
	if (stop_simu(philo))
		return ;
	print_status(philo->id, "is eating", philo);
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal_time = get_time_in_ms();
	pthread_mutex_unlock(&philo->last_meal_mutex);
	ft_usleep(philo->rules->time_to_eat);
	pthread_mutex_unlock(philo->f_right);
	pthread_mutex_unlock(philo->f_left);
	pthread_mutex_lock(&philo->nb_meals_mutex);
	philo->nb_meals++;
	pthread_mutex_unlock(&philo->nb_meals_mutex);
}

void	even_take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->f_right);
	print_status(philo->id, HAS_TAKEN_FORK, philo);
	//if (stop_simu(philo) || philo->rules->nb_philos == 1)
	if (stop_simu(philo))
	{
		pthread_mutex_unlock(philo->f_right);
		return ;
	}
	pthread_mutex_lock(philo->f_left);
	print_status(philo->id, HAS_TAKEN_FORK, philo);
	if (stop_simu(philo))
	{
		pthread_mutex_unlock(philo->f_right);
		pthread_mutex_unlock(philo->f_left);
		return ;
	}
}

void	odd_take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->f_left);
	print_status(philo->id, HAS_TAKEN_FORK, philo);
	if (stop_simu(philo))
	{
		pthread_mutex_unlock(philo->f_left);
		return ;
	}
	pthread_mutex_lock(philo->f_right);
	print_status(philo->id, HAS_TAKEN_FORK, philo);
	if (stop_simu(philo))
	{
		pthread_mutex_unlock(philo->f_right);
		pthread_mutex_unlock(philo->f_left);
		return ;
	}
}
