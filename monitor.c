/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naperisw <naperisw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:10:00 by naperisw          #+#    #+#             */
/*   Updated: 2026/01/29 16:19:52 by naperisw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	evaluate_philo_state(t_philo *philo, int *all_philos_fed)
{
	pthread_mutex_lock(&philo->meal_mutex);
	if (get_time() - philo->last_meal_timestamp >= philo->infos->time_to_die)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		return (1);
	}
	if (philo->infos->meals_required != -1
		&& philo->meals_eaten < philo->infos->meals_required)
		*all_philos_fed = 0;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (0);
}

static void	end_simulation(t_infos *infos)
{
	pthread_mutex_lock(&infos->sim_mutex);
	infos->is_simulation_running = 0;
	pthread_mutex_unlock(&infos->sim_mutex);
}

static int	check_all_fed(t_infos *infos, int all_fed)
{
	if (infos->meals_required != -1 && all_fed)
	{
		end_simulation(infos);
		return (1);
	}
	return (0);
}

void	monitor_routine(t_infos *infos)
{
	int	i;
	int	all_philos_fed;

	while (1)
	{
		all_philos_fed = 1;
		i = -1;
		while (++i < infos->philo_count)
		{
			if (evaluate_philo_state(&infos->philo[i], &all_philos_fed))
			{
				end_simulation(infos);
				print_message(&infos->philo[i], DIED);
				return ;
			}
		}
		if (check_all_fed(infos, all_philos_fed))
			return ;
		usleep(500);
	}
}
