/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:11:45 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/19 17:07:05 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

void	start_simulation(t_rules *rules)
{
	int	i;

	rules->start_time = get_time_in_ms();
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
