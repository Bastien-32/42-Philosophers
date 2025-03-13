/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:11:45 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/13 12:07:32 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

/*void	error(char*msg)
{
	
}*/

/**
 * @brief Convert a string with ascii numbers on int numbers
 * @param str (const char *) Pointer to string to convert.
 * @return (int) The integer value of the string.
 */
int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	result = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ' )
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
		result = result * 10 + str[i++] - '0';
	result *= sign;
	return ((int)result);
}

long get_time_in_ms(void)
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void	error_malloc_philos(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->nb_philos)
		pthread_mutex_destroy(&rules->forks[i++]);
	pthread_mutex_destroy(&rules->print_mutex);
	free(rules->forks);
	free(rules->philos);
	write(2, "Error: Memory allocation failed for philosophers.\n", 51);
	exit (1);
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
		rules->philos[i].last_meal_time = rules->start_time;
		rules->philos[i].f_right = &rules->forks[i];
		rules->philos[i].f_left = &rules->forks[(i + 1) % rules->nb_philos];
		rules->philos[i].rules = rules;
		pthread_mutex_init(&rules->philos[i].meal_mutex, NULL);
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
	{
		write(2, "Error: Memory allocation failed for forks.\n", 44);
		exit (1);
	}
	i = 0;
	while (i < rules->nb_philos)
		pthread_mutex_init(&rules->forks[i++], NULL);
	pthread_mutex_init(&rules->print_mutex, NULL);
	pthread_mutex_init(&rules->death_mutex, NULL);
}

void	check_parameters(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
	{
		printf(USAGE);
		exit (1);
	}
	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) < 0 || ft_atoi(argv[i]) > 2147483647)
		{
			printf(ERROR_INT);
			exit (1);
		}
		i++;
	}
	if (argv[1][0] == '1')
	{
		printf(ERROR_ONE_PHILO);
		exit(1);
	}
}

void	error_create_threads(t_rules *rules, int i)
{
	int	j;

	j = 0;
	while (j < i)
		pthread_join(rules->philos[j++].thread, NULL);
	j = 0;
	while (j < rules->nb_philos)
		pthread_mutex_destroy(&rules->forks[j++]);
	pthread_mutex_destroy(&rules->print_mutex);
	free(rules->forks);
	free(rules->philos);
	printf(ERROR_THREAD, rules->philos[i].id);
	exit (1);
}

void	print_status(int id, char *status, t_philo *philo)
{
	long	time_passed;

	time_passed = get_time_in_ms() - philo->rules->start_time;
	pthread_mutex_lock(&philo->rules->print_mutex);
	printf("%ld %d %s\n",time_passed, id, status);
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

void	take_forks(t_philo *philo)
{
	if (philo->id == philo->rules->nb_philos \
										&& philo->rules->nb_philos % 2 != 0)
		usleep(100);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->f_right);
		print_status(philo->id, "has taken a fork", philo);
		pthread_mutex_lock(philo->f_left);
		print_status(philo->id, "has taken a fork", philo);
	}
	else
	{
		pthread_mutex_lock(philo->f_left);
		print_status(philo->id, "has taken a fork", philo);
		pthread_mutex_lock(philo->f_right);
		print_status(philo->id, "has taken a fork", philo);
	}
}

void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->f_right);
	pthread_mutex_unlock(philo->f_left);
}

void	*philos_routine(void *arg)
{
	t_philo	*philo;
	philo = (t_philo *)arg;

	if (philo->id == philo->rules->nb_philos && philo->rules->nb_philos % 2 != 0)
		usleep(100);

	while (1)
	{		
		if (philo->rules->stop_simulation)
			break ;
		take_forks(philo);
		if (philo->rules->stop_simulation)
			break ;
		print_status(philo->id, "is eating", philo);
		usleep(philo->rules->time_to_eat * 1000);
		philo->last_meal_time = get_time_in_ms();
		release_forks(philo);
		if (philo->rules->stop_simulation)
			break ;
		print_status(philo->id, "is sleeping", philo);
		usleep(philo->rules->time_to_sleep * 1000);
		if (philo->rules->stop_simulation)
			break ;
		print_status(philo->id, "is thinking", philo);
		if (philo->rules->stop_simulation)
			break ;
	}
	return (NULL);
}

void	philo_die(t_rules *rules, int i)
{
	long	current_time;
	
	print_status(rules->philos[i].id, "died", &rules->philos[i]);
	pthread_mutex_lock(&rules->death_mutex);
	rules->stop_simulation = 1;
	pthread_mutex_unlock(&rules->death_mutex);
	pthread_mutex_unlock(&rules->philos[i].meal_mutex);
}

void	*monitoring_thread(void *arg)
{
	t_rules	*rules;
	int		i;
	long	current_time;

	rules = (t_rules *)arg;
	while (1)
	{
		i = 0;
		while (i < rules->nb_philos)
		{			
			current_time = get_time_in_ms();
			pthread_mutex_lock(&rules->philos[i].meal_mutex);
			usleep(500);
			if ((current_time - (long)rules->philos[i].last_meal_time) \
														> rules->time_to_die)
			{
				philo_die(rules, i);
				return (NULL);
			}
			pthread_mutex_unlock(&rules->philos[i].meal_mutex);
			i++;
		}
		usleep(500);
	}
	return (NULL);
}

void	start_simulation(t_rules *rules)
{
	int	i;
	pthread_t	monitor_thread;

	rules->start_time = get_time_in_ms();
	if (pthread_create(&monitor_thread, NULL, monitoring_thread, rules) != 0)
	{
		printf("Error : impossible to create the survey thread.\n");
		exit(1);
	}
	i = 0;	
	while (i < rules->nb_philos)
	{
		rules->philos[i].last_meal_time = rules->start_time;
		if (pthread_create(&rules->philos[i].thread, NULL, philos_routine, \
													&rules->philos[i]) != 0)
			error_create_threads(rules, i);
		i++;		
	}
	pthread_join(monitor_thread, NULL);
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
	return(0);
}
