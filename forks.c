/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naperisw <naperisw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 11:35:56 by naperisw          #+#    #+#             */
/*   Updated: 2026/01/30 11:35:41 by naperisw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_fork_mutexes(t_infos *infos, int count)
{
	while (--count >= 0)
		pthread_mutex_destroy(&infos->fork_mutexes[count]);
	free(infos->fork_mutexes);
}

int	lock_and_take_fork(t_philo *philo, pthread_mutex_t *fork)
{
	if (!simulation_active(philo->infos))
		return (0);
	pthread_mutex_lock(fork);
	if (!simulation_active(philo->infos))
	{
		pthread_mutex_unlock(fork);
		return (0);
	}
	print_message(philo, FORK);
	return (1);
}

int	lock_forks_in_order(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (philo->left_idx < philo->right_idx)
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	else
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	if (!lock_and_take_fork(philo, first))
		return (0);
	if (!lock_and_take_fork(philo, second))
	{
		pthread_mutex_unlock(first);
		return (0);
	}
	return (1);
}
