/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naperisw <naperisw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 16:19:43 by naperisw          #+#    #+#             */
/*   Updated: 2026/01/29 15:49:09 by naperisw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_philo *philo)
{
	int				finished;

	if (!lock_forks_in_order(philo))
		return ;
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_timestamp = get_time();
	pthread_mutex_unlock(&philo->meal_mutex);
	print_message(philo, EAT);
	finished = smart_sleep(philo->infos->time_to_eat, philo->infos);
	pthread_mutex_lock(&philo->meal_mutex);
	if (finished)
		philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void	philo_think(t_philo *philo)
{
	long	think_delay;
	long	slack;

	print_message(philo, THINK);
	if (philo->infos->philo_count % 2 != 0)
	{
		slack = philo->infos->time_to_die - philo->infos->time_to_eat
			- philo->infos->time_to_sleep;
		if (slack <= 0)
			think_delay = 0;
		else if (philo->infos->philo_count <= 3)
			think_delay = slack / 3;
		else
			think_delay = slack / 5;
		if (think_delay > 0)
			smart_sleep(think_delay, philo->infos);
	}
}

void	*philo_lifecycle(void *philo_data)
{
	t_philo	*philo;

	philo = (t_philo *)philo_data;
	if (philo->id % 2 != 0)
		smart_sleep(philo->infos->time_to_eat / 2, philo->infos);
	while (simulation_active(philo->infos))
	{
		if (!check_meal_ready(philo))
			break ;
		philo_eat(philo);
		if (!simulation_active(philo->infos))
			break ;
		print_message(philo, SLEEP);
		smart_sleep(philo->infos->time_to_sleep, philo->infos);
		if (!simulation_active(philo->infos))
			break ;
		philo_think(philo);
	}
	return (NULL);
}

void	*philo_routine(void *p_data)
{
	t_philo	*philo;

	philo = (t_philo *)p_data;
	wait_and_start(philo);
	if (philo->infos->philo_count == 1)
	{
		print_message(philo, FORK);
		smart_sleep(philo->infos->time_to_die, philo->infos);
		return (NULL);
	}
	philo_lifecycle(philo);
	return (NULL);
}
