/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:11:45 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/06 19:43:45 by badal-la         ###   ########.fr       */
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

void *philos_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    printf("Philosophe %d commence\n", philo->id);
    return (NULL);
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
	if (argv[5])
		rules->nb_meat = ft_atoi(argv[5]);
	else
		rules->nb_meat = -1;
	rules->start_time = get_time_in_ms(); // a deplacer dans la fonction ou on cree les thread, ici il y a des ms qui vont se perdre le tempsde faire les init
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
}

void	check_parameters(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
	{
		write(2, "Usage: ./philo number_of_philos time_die time_eat "
              "time_sleep [meals]\n", 70);
		exit (1);
	}
	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) < 0 || ft_atoi(argv[i]) > 2147483647)
		{
			write(2, "Error: Enter positive int (0 to 2147483647).\n", 46);
			exit (1);
		}
		i++;
	}
	if (argv[1][0] == '1')
	{
		write(2, "Error: Only one fork available.\n"
			"The philosopher will starve.\n", 62);
		exit(1);
	}
}

int	main(int argc, char **argv)
{
	t_rules rules;
	int		i;

	check_parameters(argc, argv);
	init_rules(argv, &rules);
	init_philos(&rules);
	//start_simulation(&rules)
	//ce qui est dessous est a integrer dans les fct qui quittent le programme
	// fin de programme ou mort philo ou erreur apres leur init
	i = 0;
	while (i < rules.nb_philos)
		pthread_join(rules.philos[i++].thread, NULL);
	i = 0;
	while (i < rules.nb_philos)
		pthread_mutex_destroy(&rules.forks[i++]);
	pthread_mutex_destroy(&rules.print_mutex);
	free(rules.forks);
	free(rules.philos);
	return(0);
}
