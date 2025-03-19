/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_check_param.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:29:29 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/19 13:58:01 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_parameters(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
		error_exit(USAGE);
	i = 1;
	while (i < argc)
	{
		if (!is_number(argv[i]))
			error_exit(FILL_WITH_NUMBERS);
		if (ft_atoi(argv[i]) < 0 || ft_atoi(argv[i]) > 2147483647)
			error_exit(ERROR_INT);
		i++;
	}
	if (argv[1][0] == '1')
		error_exit(ERROR_ONE_PHILO);
}

void	init_philos(t_rules *rules)
{
	int	i;

	rules->philos = malloc(sizeof(t_philo) * rules->nb_philos);
	if (!rules->philos)
		error_malloc_philos(rules);
	i = 0;
	while (i < rules->nb_philos)
	{
		rules->philos[i].id = i + 1;
		rules->philos[i].f_right = &rules->forks[i];
		rules->philos[i].f_left = &rules->forks[(i + 1) % rules->nb_philos];
		rules->philos[i].rules = rules;
		rules->philos[i].nb_meals = 0;
		pthread_mutex_init(&rules->philos[i].last_meal_mutex, NULL);
		pthread_mutex_init(&rules->philos[i].nb_meals_mutex, NULL);
		i++;
	}
}

void	init_rules(char **argv, t_rules *rules)
{
	int	i;

	rules->nb_philos = ft_atoi(argv[1]);
	rules->time_to_die = ft_atoi(argv[2]);
	rules->time_to_eat = ft_atoi(argv[3]);
	rules->time_to_sleep = ft_atoi(argv[4]);
	rules->stop_simulation = 0;
	if (argv[5])
		rules->nb_meat = ft_atoi(argv[5]);
	else
		rules->nb_meat = -1;
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->nb_philos);
	if (!rules->forks)
		error_exit(ERROR_MALLOC_FORKS);
	i = 0;
	while (i < rules->nb_philos)
		pthread_mutex_init(&rules->forks[i++], NULL);
	pthread_mutex_init(&rules->print_mutex, NULL);
	pthread_mutex_init(&rules->death_mutex, NULL);
}
