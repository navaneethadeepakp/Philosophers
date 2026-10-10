/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naperisw <naperisw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 15:16:03 by naperisw          #+#    #+#             */
/*   Updated: 2026/01/27 12:36:35 by naperisw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	cleanup_threads(t_infos *infos, int created)
{
	int	i;

	pthread_mutex_lock(&infos->sim_mutex);
	infos->is_simulation_running = 0;
	pthread_mutex_unlock(&infos->sim_mutex);
	i = 0;
	while (i < created)
	{
		pthread_join(infos->philo[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_infos	infos;
	int		i;

	if (init_infos(argc, argv, &infos) != 0)
		return (1);
	i = 0;
	while (i < infos.philo_count)
	{
		if (pthread_create(&infos.philo[i].thread, NULL, philo_routine,
				&infos.philo[i]) != 0)
		{
			cleanup_threads(&infos, i);
			destroy_mutexes(&infos);
			return (1);
		}
		i++;
	}
	start_simulation(&infos);
	monitor_routine(&infos);
	destroy_program(&infos);
	return (0);
}
