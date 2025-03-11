/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:12:25 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/11 09:56:38 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>


/* ****************************************************************************
								  MSG PRINTF
**************************************************************************** */

# define USAGE "Usage: ./philo number_of_philos time_die \
time_eat time_sleep [meals]\n"
# define ERROR_MALLOC_FORKS "Error: Memory allocation failed for forks.\n"
# define ERROR_MALLOC_PHILOS "Error: Memory allocation failed for philosophers.\n"
# define ERROR_THREAD "Error : Impossible to create the thread of philosopher %d\n"
# define ERROR_INT "Error: Enter positive int (0 to 2147483647).\n"
# define ERROR_ONE_PHILO "Error: Only one fork available.\n" \


typedef struct s_rules t_rules; 

typedef struct s_rules
{
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long			nb_meat;
	long			start_time;
	struct s_philo	*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
}					t_rules;

typedef struct s_philo
{
	int				id;
	long			last_meal_time;
	pthread_mutex_t	meal_mutex;
	pthread_t		thread;
	pthread_mutex_t	*f_right;
	pthread_mutex_t	*f_left;
	t_rules			*rules;
}					t_philo;

void	init_philos(t_rules *rules);
void	init_rules(char **argv, t_rules *rules);

#endif