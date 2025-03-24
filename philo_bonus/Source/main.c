/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:11:45 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/24 18:42:33 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	only_one_philo(t_rules *rules)
{
	long	current_time;

	if (rules->nb_philos == 1)
	{
		current_time = get_time_in_ms() - rules->philos->rules->start_time;
		printf("%ld 1 has taken a fork\n", current_time);
		ft_usleep(rules->time_to_die);
		current_time = get_time_in_ms() - rules->philos->rules->start_time;
		printf("%ld 1 died\n", current_time);
		close_sem_rules(rules);
		sem_close(rules->philos[0].last_meal_sem);
		sem_unlink("/last_meal_time 0");
		sem_close(rules->philos[0].nb_meals_sem);
		sem_unlink("/nb_meals 0");
		free(rules->philos);
		return (1);
	}
	return (0);
}

int	get_philo_index_by_pid(pid_t pid, t_rules *rules)
{
	int i = 0;
	while (i < rules->nb_philos)
	{
		if (rules->philos[i].pid == pid)
			return (i);
		i++;
	}
	return (-1);
}

void	wait_for_termination(t_rules *rules)
{
	int		status;
	pid_t	pid;
	int		nb_exited;
	int		i;

	nb_exited = 0;
	while (1)
	{
		pid = waitpid(-1, &status, 0);
		//printf("[DEBUG] Process terminé → pid = %d\n", pid);
		if (pid == -1)
			error_exit("waitpid failed");
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 1)
			{
				i = get_philo_index_by_pid(pid, rules);
				//printf("[DEBUG] Ce pid correspond au philosophe id = %d\n", rules->philos[i].id);
				long now = get_time_in_ms() - rules->start_time;
				sem_post(rules->print_sem);
				sem_wait(rules->print_sem);
				printf("%ld %d died\n", now, rules->philos[i].id);
				sem_post(rules->print_sem);
				i = 0;
				while (i < rules->nb_philos)
					kill(rules->philos[i++].pid, SIGTERM);
				break ;
			}
			else if (WEXITSTATUS(status) == 0)
				nb_exited++;
			if (rules->nb_meat > 0 && nb_exited == rules->nb_philos)
			{
				long t = get_time_in_ms() - rules->start_time;
				sem_wait(rules->print_sem);
				printf(END_AFTER_N_MEALS, t, rules->nb_meat);
				sem_post(rules->print_sem);
				break ;
			}
		}
	}
}

void	start_simulation(t_rules *rules)
{
	int	i;

	
	rules->start_time = get_time_in_ms();
	if (only_one_philo(rules))
		exit (1);
	i = 0;
	while (i < rules->nb_philos)
	{
		rules->philos[i].pid = fork();
		if (rules->philos[i].pid == -1)
			error_exit("Fork failed");
		if (rules->philos[i].pid == 0)
		{
			philos_routine(&rules->philos[i]);
			exit(0);
		}
		sem_wait(rules->philos[i].last_meal_sem);
		rules->philos[i].last_meal_time = rules->start_time;
		sem_post(rules->philos[i].last_meal_sem);
		i++;
	}
	wait_for_termination(rules);
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
