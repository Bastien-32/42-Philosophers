/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:12:25 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/18 18:16:07 by badal-la         ###   ########.fr       */
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
								   MESSAGES
**************************************************************************** */

# define USAGE "Usage: ./philo number_of_philos time_die \
time_eat time_sleep [meals]\n"
# define ERROR_MALLOC_FORKS "Error: Memory allocation failed for forks.\n"
# define ERROR_MALLOC_PHILOS "Error: Memory allocation failed for \
philosophers.\n"
# define ERROR_MONITOR "Error : impossible to create the survey thread.\n"
# define ERROR_THREAD "Error : Impossible to create the thread of \
philosopher %d\n"
# define FILL_WITH_NUMBERS "Error: fill parameters only with numbers\n"
# define ERROR_INT "Error: Enter positive int (0 to 2147483647).\n"
# define ERROR_ONE_PHILO "Error: Only one fork available.\n"
# define HAS_TAKEN_FORK "has taken a fork"
# define END_AFTER_N_MEALS "%ld All philo have eaten %ld time\n"

typedef struct s_rules t_rules; 

typedef struct s_rules
{
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long			nb_meat;
	long			start_time;
	int				stop_simulation;
	int				simulation_end;
	struct s_philo	*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_t 		monitor_thread;
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
	int				nb_meals;
}					t_philo;

/* ****************************************************************************
								   errors.c
**************************************************************************** */

void	error_exit(const char *message);
void	error_malloc_philos(t_rules *rules);
void	error_create_monitor(t_rules *rules);
void	error_create_threads(t_rules *rules, int i);

/* ****************************************************************************
							init_and_check_param.c
**************************************************************************** */

void	check_parameters(int argc, char **argv);
void	init_philos(t_rules *rules);
void	init_rules(char **argv, t_rules *rules);

/* ****************************************************************************
								 monitoring.c
**************************************************************************** */

void	*monitoring_thread(void *arg);
void	check_philosophers(t_rules *rules);
int		philo_has_died(t_rules *rules, int i);
void	philo_die(t_rules *rules, int i);
int		all_philos_full(t_rules *rules);

/* ****************************************************************************
								philo_routine.c
**************************************************************************** */

void	*philos_routine(void *arg);
void	philo_is_eating(t_philo *philo);
void	take_forks(t_philo *philo);
void	release_forks(t_philo *philo);

/* ****************************************************************************
									utils.c
**************************************************************************** */

int		ft_atoi(const char *str);
int		ft_isdigit(int c);
int		is_number(const char *str);
void	ft_usleep(long ms);
long 	get_time_in_ms(void);

/* ****************************************************************************
								   utils2.c
**************************************************************************** */

void	print_status(int id, char *status, t_philo *philo);
void	quit_program(t_rules *rules, long time_passed, char *msg);

#endif