/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:12:25 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/20 21:26:57 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/stat.h> 

/* ****************************************************************************
								   MESSAGES
**************************************************************************** */

# define USAGE "Usage: ./philo number_of_philos time_die \
time_eat time_sleep [meals]\n"
# define ERROR_SEM_FORKS "Error: Fail to create semaphores for forks.\n"
# define ERROR_MALLOC_PHILOS "Error: Memory allocation failed for \
philosophers.\n"
# define ERROR_MONITOR "Error : impossible to create the survey thread.\n"
# define ERROR_THREAD "Error : Impossible to create the thread of \
philosopher %d\n"
# define FILL_WITH_NUMBERS "Error: fill parameters only with numbers\n"
# define ERROR_INT "Error: Enter positive int (0 to 2147483647).\n"
# define HAS_TAKEN_FORK "has taken a fork"
# define END_AFTER_N_MEALS "%ld All philo have eaten %ld time\n"
# define ERROR_SEM_LAST_MEAL_TIME "Error in sem_open for last_meal_sem"
# define ERROR_SEM_NB_MEALS "Error in sem_open for nb_meal_sem"

typedef struct s_rules	t_rules;

typedef struct s_rules
{
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long			nb_meat;
	long			start_time;
	int				stop_simulation;
	struct s_philo	*philos;
	sem_t			*forks;
	sem_t			*print_sem;
	sem_t			*death_sem;
	pthread_t		monitor_thread;
}					t_rules;

typedef struct s_philo
{
	int				id;
	long			last_meal_time;
	sem_t			*last_meal_sem;
	pthread_t		thread;
	t_rules			*rules;
	int				nb_meals;
	sem_t			*nb_meals_sem;
}					t_philo;

/* ****************************************************************************
								   errors.c
**************************************************************************** */

void	error_exit(const char *message);
void	error_malloc_philos(t_rules *rules);

/* ****************************************************************************
							init_and_check_param.c
**************************************************************************** */

void	check_parameters(int argc, char **argv);
void	fill_basic_variables(t_rules *rules, int i);
void	init_philos(t_rules *rules);
void	init_rules(char **argv, t_rules *rules);

/* ****************************************************************************
								 monitoring.c
**************************************************************************** */

void	*monitoring_thread(void *arg);
void	check_philosophers(t_rules *rules);
int		philo_died(t_rules *rules, int i);
int		all_philos_full(t_rules *rules);

/* ****************************************************************************
								philo_routine.c
**************************************************************************** */

int		stop_simu(t_philo *philo);
void	*philos_routine(void *arg);
void	philo_is_eating(t_philo *philo);

/* ****************************************************************************
									utils.c
**************************************************************************** */

int		ft_atoi(const char *str);
int		ft_isdigit(int c);
int		is_number(const char *str);
void	ft_usleep(long ms);
long	get_time_in_ms(void);

/* ****************************************************************************
								   utils2.c
**************************************************************************** */

void	print_status(int id, char *status, t_philo *philo);
void	print_status_stop_simu(int id, char *status, t_philo *philo);
void	quit_program(t_rules *rules);
void	close_sem_rules(t_rules *rules);

/* ****************************************************************************
								   utils3.c
**************************************************************************** */

size_t	ft_strlen(const char *s);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_itoa(int n);

#endif