/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:11:45 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/05 14:34:53 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

/*void	error(char*msg)
{
	
}*/

long get_time_in_ms(void)
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void init_philos(t_rules *rules)
{
	int	i;

	rules->philos = malloc(sizeof(t_philo) * rules->nb_philos);
	if (!rules->philos)
	{
		printf("Error: Memory allocation failed for philosophers.\n");
		return ;
	}
	i = 0;
	while (i < rules->nb_philos)
	{
		rules->philos[i].id = i + 1;
		rules->philos[i].last_meal_time = rules->start_time;
		rules->philos[i].f_right = &rules->forks[i];
		rules->philos[i].f_left = &rules->forks[(i + 1) % rules->nb_philos];
		i++;
	}
}

void	init_rules(char **argv, t_rules *rules)
{
	int	i;
	
	rules->nb_philos = argv[1];
	rules->time_to_die = argv[2];
	rules->time_to_eat = argv[3];
	rules->time_to_sleep = argv[4];
	if (argv[5])
		rules->nb_meat = argv[5];
	else
		rules->nb_meat = -1;
	rules->start_time = get_time_in_ms();
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->nb_philos);
	if (!rules->forks)
	{
		printf("Error: Memory allocation failed for forks.\n");
		return ;
	}
	i = 0;
	while (i < rules->nb_philos)
		pthread_mutex_init(&rules->forks[i++], NULL);
	pthread_mutex_init(&rules->print_mutex, NULL);
}

int	main(int argc, char **argv)
{
	t_rules rules;
	
	(void)argv;
	
	if (argc < 5 || argc > 6)
	{
		printf("Usage : number_of_philosophers time_to_die time_to_eat ");
		printf("time_to_sleep [number_of_times_each_philosopher_must_eat]");
		return (0);
	}
	init_rules(argv, &rules);
	init_philos(&rules);

	int	i = 0;
	while (i < rules.nb_philos)
		pthread_mutex_destroy(&rules.forks[i++]);
	free(rules.forks);
	free(rules.philos);
	return(0);
}
