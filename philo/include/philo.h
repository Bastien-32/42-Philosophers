/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:12:25 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/06 14:45:42 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>


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
	pthread_t		thread;
	pthread_mutex_t	*f_right;
	pthread_mutex_t	*f_left;
	t_rules			*rules;
}					t_philo;

void	init_philos(t_rules *rules);
void	init_rules(char **argv, t_rules *rules);

#endif