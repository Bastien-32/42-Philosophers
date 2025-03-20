/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:11:45 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/20 15:57:01 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
		sem_close(rules->forks);
		sem_unlink("/forks");
		sem_close(rules->philos[0].last_meal_sem);
		sem_unlink("/last_meal_time 0");
		sem_close(rules->philos[0].nb_meals_sem);
		sem_unlink("/nb_meals 0");
		sem_close(rules->print_sem);
		sem_unlink("/print");
		sem_close(rules->death_sem);
		sem_unlink("/death");
		free(rules->philos);
		return (1);
	}
	return (0);
}

void	start_simulation(t_rules *rules)
{
	int	i;

	rules->start_time = get_time_in_ms();
	if (only_one_philo(rules))
		exit (1);
	i = 0;
	printf("Nombre de philosophes : %d\n", rules->nb_philos);
	while (i < rules->nb_philos)
	{
		printf("Avant sem_wait: philo %d\n", i);
		sem_wait(rules->philos[i].last_meal_sem);
		printf("Apres sem_wait: philo %d\n", i);
		rules->philos[i].last_meal_time = rules->start_time;
		sem_post(rules->philos[i].last_meal_sem);
		i++;
	}
	printf("uhu\n");
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
	t_rules	rules;

	check_parameters(argc, argv);
	init_rules(argv, &rules);
	init_philos(&rules);
	start_simulation(&rules);
	quit_program(&rules);
	return (0);
}
