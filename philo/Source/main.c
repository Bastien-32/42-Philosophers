/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:11:45 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/18 17:38:50 by badal-la         ###   ########.fr       */
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
		rules->philos[i++].last_meal_time = rules->start_time;
	if (pthread_create(&rules->monitor_thread, NULL, monitoring_thread, rules) != 0)
		error_create_monitor(rules);
	i = 0;	
	while (i < rules->nb_philos)
	{
		if (pthread_create(&rules->philos[i].thread, NULL, philos_routine, \
													&rules->philos[i]) != 0)
			error_create_threads(rules, i);
		i++;		
	}
	usleep(200);
	pthread_join(rules->monitor_thread, NULL);
	i = 0;
	while (i < rules->nb_philos)
		pthread_join(rules->philos[i++].thread, NULL);
}

int	main(int argc, char **argv)
{
	t_rules rules;

	check_parameters(argc, argv);
	init_rules(argv, &rules);
	init_philos(&rules);
	start_simulation(&rules);
	return (0);
}
