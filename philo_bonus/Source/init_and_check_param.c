/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_check_param.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:29:29 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/20 15:45:06 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
}

void	init_philos(t_rules *rules)
{
	int	i;
	char *sem_name;

	rules->philos = malloc(sizeof(t_philo) * rules->nb_philos);
	if (!rules->philos)
		error_malloc_philos(rules);
	i = 0;
	while (i < rules->nb_philos)
	{
		sem_name = ft_strjoin("/last_meal_time ", ft_itoa(i));
		rules->philos[i].id = i + 1;
		rules->philos[i].rules = rules;
		rules->philos[i].nb_meals = 0;
		rules->philos[i].last_meal_sem = sem_open(sem_name, O_CREAT, 0644, 1);
		if (rules->philos[i].last_meal_sem == SEM_FAILED)
			error_exit(ERROR_SEM_LAST_MEAL_TIME);
		free(sem_name);
		sem_name = ft_strjoin("/nb_meals ", ft_itoa(i));
		rules->philos[i].nb_meals_sem = sem_open(sem_name, O_CREAT, \
																	0644, 1);
		if (rules->philos[i].nb_meals_sem == SEM_FAILED)
			error_exit(ERROR_SEM_NB_MEALS);
		free(sem_name);
		i++;
	}
}

void	init_rules(char **argv, t_rules *rules)
{
	rules->nb_philos = ft_atoi(argv[1]);
	rules->time_to_die = ft_atoi(argv[2]);
	rules->time_to_eat = ft_atoi(argv[3]);
	rules->time_to_sleep = ft_atoi(argv[4]);
	rules->stop_simulation = 0;
	if (argv[5])
		rules->nb_meat = ft_atoi(argv[5]);
	else
		rules->nb_meat = -1;
	rules->forks = sem_open("/forks", O_CREAT, 0644, rules->nb_philos / 2);
	if (rules->forks == SEM_FAILED)
		error_exit(ERROR_SEM_FORKS);
	rules->print_sem = sem_open("/print", O_CREAT, 0644, 1);
	rules->death_sem = sem_open("/death", O_CREAT, 0644, 1);
}
