/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infos.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naperisw <naperisw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 15:53:58 by naperisw          #+#    #+#             */
/*   Updated: 2026/01/29 14:59:38 by naperisw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_philo_infos(t_infos *infos)
{
	int	i;

	i = 0;
	while (i < infos->philo_count)
	{
		infos->philo[i].id = i + 1;
		infos->philo[i].meals_eaten = 0;
		infos->philo[i].last_meal_timestamp = 0;
		infos->philo[i].infos = infos;
		if (pthread_mutex_init(&infos->philo[i].meal_mutex, NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&infos->philo[i].meal_mutex);
			return (1);
		}
		infos->philo[i].left_idx = i;
		infos->philo[i].right_idx = (i + 1) % infos->philo_count;
		infos->philo[i].left_fork
			= &infos->fork_mutexes[infos->philo[i].left_idx];
		infos->philo[i].right_fork
			= &infos->fork_mutexes[infos->philo[i].right_idx];
		i++;
	}
	return (0);
}

static int	init_philo_infos_fail(t_infos *infos)
{
	int	i;

	i = 0;
	while (i < infos->philo_count)
	{
		pthread_mutex_destroy(&infos->fork_mutexes[i]);
		i++;
	}
	pthread_mutex_destroy(&infos->write_mutex);
	pthread_mutex_destroy(&infos->sim_mutex);
	pthread_mutex_destroy(&infos->start_mutex);
	free(infos->fork_mutexes);
	free(infos->philo);
	return (1);
}

static	int	init_mutexes(t_infos *infos)
{
	int	i;

	i = 0;
	infos->fork_mutexes = malloc(sizeof(pthread_mutex_t) * infos->philo_count);
	if (!infos->fork_mutexes)
		return (1);
	while (i < infos->philo_count)
	{
		if (pthread_mutex_init(&infos->fork_mutexes[i], NULL) != 0)
			return (destroy_fork_mutexes(infos, i), 1);
		i++;
	}
	if (pthread_mutex_init(&infos->write_mutex, NULL) != 0)
		return (destroy_fork_mutexes(infos, i), 1);
	if (pthread_mutex_init(&infos->sim_mutex, NULL) != 0)
		return (pthread_mutex_destroy(&infos->write_mutex),
			destroy_fork_mutexes(infos, i), 1);
	if (pthread_mutex_init(&infos->start_mutex, NULL) != 0)
		return (pthread_mutex_destroy(&infos->sim_mutex),
			pthread_mutex_destroy(&infos->write_mutex),
			destroy_fork_mutexes(infos, i), 1);
	return (0);
}

static int	parse_args(int argc, char **argv, t_infos *infos)
{
	if (argc < 5 || argc > 6)
		return (printf("Error: invalid number of arguments\n"), 1);
	infos->philo_count = ft_satoi(argv[1]);
	infos->time_to_die = ft_satoi(argv[2]);
	infos->time_to_eat = ft_satoi(argv[3]);
	infos->time_to_sleep = ft_satoi(argv[4]);
	infos->meals_required = -1;
	if (argc == 6)
		infos->meals_required = ft_satoi(argv[5]);
	if (infos->philo_count <= 0 || infos->time_to_die <= 0
		|| infos->time_to_eat <= 0 || infos->time_to_sleep <= 0
		|| (argc == 6 && infos->meals_required <= 0))
		return (printf("Error: invalid arguments\n"), 1);
	return (0);
}

int	init_infos(int argc, char **argv, t_infos *infos)
{
	if (parse_args(argc, argv, infos) != 0)
		return (1);
	infos->is_simulation_running = 1;
	infos->sim_start = 0;
	infos->start_flag = 0;
	infos->philo = malloc(sizeof(t_philo) * infos->philo_count);
	if (!infos->philo)
		return (1);
	if (init_mutexes(infos) != 0)
		return (free(infos->philo), 1);
	if (init_philo_infos(infos) != 0)
		return (init_philo_infos_fail(infos));
	return (0);
}
