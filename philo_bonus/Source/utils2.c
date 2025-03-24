/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:38:24 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/24 09:04:09 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	print_status(int id, char *status, t_philo *philo)
{
	long	time_passed;

	time_passed = get_time_in_ms() - philo->rules->start_time;
	sem_wait(philo->rules->death_sem);
	if (!philo->rules->stop_simulation)
	{
		sem_wait(philo->rules->print_sem);
		printf("%ld %d %s\n", time_passed, id, status);
		sem_post(philo->rules->print_sem);
		sem_post(philo->rules->death_sem);
		return ;
	}
	sem_post(philo->rules->death_sem);
}

void	print_status_stop_simu(int id, char *status, t_philo *philo)
{
	long	time_passed;

	time_passed = get_time_in_ms() - philo->rules->start_time;
	sem_wait(philo->rules->death_sem);
	if (!philo->rules->stop_simulation)
	{
		sem_wait(philo->rules->print_sem);
		printf("%ld %d %s\n", time_passed, id, status);
		sem_post(philo->rules->print_sem);
		sem_post(philo->rules->death_sem);
		return ;
	}
	sem_post(philo->rules->death_sem);
}

void	quit_program(t_rules *rules)
{
	int		i;
	char	*sem_name;

	i = 0;
	usleep(200);
	pthread_join(rules->monitor_thread, NULL);
	i = 0;
	while (i < rules->nb_philos)
	{
		sem_name = ft_strjoin("/last_meal_time ", ft_itoa(i));
		sem_close(rules->philos[i].last_meal_sem);
		sem_unlink(sem_name);
		free(sem_name);
		sem_name = ft_strjoin("/nb_meals ", ft_itoa(i));
		sem_close(rules->philos[i].nb_meals_sem);
		sem_unlink(sem_name);
		free(sem_name);
		i++;
	}
	close_sem_rules(rules);
	free(rules->philos);
}

void	close_sem_rules(t_rules *rules)
{
	sem_close(rules->forks);
	sem_unlink("/forks");
	sem_close(rules->print_sem);
	sem_unlink("/print");
	sem_close(rules->death_sem);
	sem_unlink("/death");
}
