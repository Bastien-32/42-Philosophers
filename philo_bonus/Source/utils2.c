/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:38:24 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/20 15:31:43 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	print_status(int id, char *status, t_philo *philo)
{
	long	time_passed;

	time_passed = get_time_in_ms() - philo->rules->start_time;
	sem_wait(philo->rules->death_sem);
	if (!philo->rules->stop_simulation)
	{
		sem_wait(philo->rules->print_sem);
		printf("%ld %d %s\n", time_passed, id, status);
		sem_post(philo->rules->print_sem);
		sem_post(philo->rules->death_sem);
		return ;
	}
		sem_post(philo->rules->death_sem);
}

void	quit_program(t_rules *rules)
{
	int		i;
	char	*sem_name;

	i = 0;
	usleep(200);
	pthread_join(rules->monitor_thread, NULL);
	while (i < rules->nb_philos)
		pthread_join(rules->philos[i++].thread, NULL);
	sem_close(rules->forks);
	sem_unlink("/forks");
	i = 0;
	while (i < rules->nb_philos)
	{
		sem_name = ft_strjoin("/last_meal_time ", ft_itoa(i));
		sem_close(rules->philos[i].last_meal_sem);
		sem_unlink(sem_name);
		free(sem_name);
		sem_name = ft_strjoin("/nb_meals ", ft_itoa(i));
		sem_close(rules->philos[i].nb_meals_sem);
		sem_unlink(sem_name);
		free(sem_name);
		i++;
	}
	sem_close(rules->print_sem);
	sem_unlink("/print");
	sem_close(rules->death_sem);
	sem_unlink("/death");
	free(rules->philos);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*dest;

	i = 0;
	j = 0;
	dest = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		dest[i + j] = s2[j];
		j++;
	}
	dest[i + j] = '\0';
	free(s2);
	return (dest);
}

static int	ft_lenn(int n)
{
	int	i;

	i = 0;
	if (n <= 0)
		i = 1;
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		i;
	long	nb;

	nb = n;
	i = ft_lenn(nb);
	str = (char *)malloc((i + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[i] = '\0';
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	if (n == 0)
		str[0] = '0';
	while (nb > 0)
	{
		i--;
		str[i] = (nb % 10) + '0';
		nb /= 10;
	}
	return (str);
}

