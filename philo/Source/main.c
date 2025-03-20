/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:11:45 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/20 10:08:43 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

int	only_one_philo(t_rules *rules)
{
	int		i;
	long	current_time;

	i = 0;
	if (rules->nb_philos == 1)
	{
		current_time = get_time_in_ms() - rules->philos->rules->start_time;
		printf("%ld 1 has taken a fork\n", current_time);
		ft_usleep(rules->time_to_die); 
		current_time = get_time_in_ms() - rules->philos->rules->start_time;
		printf("%ld 1 died\n", current_time);
		while (i < rules->nb_philos)
		{
			pthread_mutex_destroy(&rules->forks[i]);
			pthread_mutex_destroy(&rules->philos[i].last_meal_mutex);
			pthread_mutex_destroy(&rules->philos[i].nb_meals_mutex);
			i++;
		}
		pthread_mutex_destroy(&rules->print_mutex);
		pthread_mutex_destroy(&rules->death_mutex);
		free(rules->forks);
		free(rules->philos);
		return (1);
	}
	return(0);
}

void	start_simulation(t_rules *rules)
{
	int	i;

	rules->start_time = get_time_in_ms();
	if(only_one_philo(rules))
		exit (1);
	i = 0;
	while (i < rules->nb_philos)
	{
		pthread_mutex_lock(&rules->philos[i].last_meal_mutex);
		rules->philos[i].last_meal_time = rules->start_time;
		pthread_mutex_unlock(&rules->philos[i].last_meal_mutex);
		i++;
	}
	pthread_create(&rules->monitor_thread, NULL, monitoring_thread, rules);
	i = 0;	
	while (i < rules->nb_philos)
	{
		pthread_create(&rules->philos[i].thread, NULL, philos_routine, \
															&rules->philos[i]);
		i++;		
	}
}

int	main(int argc, char **argv)
{
	t_rules rules;

	check_parameters(argc, argv);
	init_rules(argv, &rules);
	init_philos(&rules);
	start_simulation(&rules);
	quit_program(&rules);
	return (0);
}
