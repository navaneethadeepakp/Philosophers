/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naperisw <naperisw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 16:18:55 by naperisw          #+#    #+#             */
/*   Updated: 2026/01/27 11:20:26 by naperisw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutexes(t_infos *info)
{
	int	i;

	i = 0;
	while (i < info->philo_count)
	{
		pthread_mutex_destroy(&info->fork_mutexes[i]);
		pthread_mutex_destroy(&info->philo[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&info->write_mutex);
	pthread_mutex_destroy(&info->sim_mutex);
	pthread_mutex_destroy(&info->start_mutex);
	free(info->fork_mutexes);
	free(info->philo);
}

void	destroy_program(t_infos *info)
{
	int	i;

	i = 0;
	while (i < info->philo_count)
	{
		pthread_join(info->philo[i].thread, NULL);
		i++;
	}
	destroy_mutexes(info);
}

long long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	simulation_active(t_infos *infos)
{
	int	active;

	pthread_mutex_lock(&infos->sim_mutex);
	active = infos->is_simulation_running;
	pthread_mutex_unlock(&infos->sim_mutex);
	return (active);
}

void	print_message(t_philo *philo, const char *msg)
{
	long long	time;

	pthread_mutex_lock(&philo->infos->sim_mutex);
	if (!philo->infos->is_simulation_running && ft_strncmp(msg, DIED, 4) != 0)
	{
		pthread_mutex_unlock(&philo->infos->sim_mutex);
		return ;
	}
	pthread_mutex_lock(&philo->infos->write_mutex);
	time = get_time() - philo->infos->sim_start;
	printf("%lld %d %s\n", time, philo->id, msg);
	pthread_mutex_unlock(&philo->infos->write_mutex);
	pthread_mutex_unlock(&philo->infos->sim_mutex);
}
