/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_philos.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naperisw <naperisw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 11:27:34 by naperisw          #+#    #+#             */
/*   Updated: 2026/01/27 12:49:12 by naperisw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_and_start(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->infos->start_mutex);
		if (philo->infos->start_flag)
		{
			pthread_mutex_unlock(&philo->infos->start_mutex);
			return ;
		}
		pthread_mutex_unlock(&philo->infos->start_mutex);
		pthread_mutex_lock(&philo->infos->sim_mutex);
		if (!philo->infos->is_simulation_running)
		{
			pthread_mutex_unlock(&philo->infos->sim_mutex);
			return ;
		}
		pthread_mutex_unlock(&philo->infos->sim_mutex);
		usleep(100);
	}
}

void	start_simulation(t_infos *infos)
{
	int	i;

	infos->sim_start = get_time();
	i = 0;
	while (i < infos->philo_count)
	{
		pthread_mutex_lock(&infos->philo[i].meal_mutex);
		infos->philo[i].last_meal_timestamp = infos->sim_start;
		pthread_mutex_unlock(&infos->philo[i].meal_mutex);
		i++;
	}
	pthread_mutex_lock(&infos->start_mutex);
	infos->start_flag = 1;
	pthread_mutex_unlock(&infos->start_mutex);
}
