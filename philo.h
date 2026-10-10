/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naperisw <naperisw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 15:16:38 by naperisw          #+#    #+#             */
/*   Updated: 2026/01/29 15:48:56 by naperisw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/time.h>

# define FORK	"has taken a fork"
# define EAT	"is eating"
# define SLEEP	"is sleeping"
# define THINK	"is thinking"
# define DIED 	"died"

typedef struct s_infos
{
	long long		sim_start;
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_required;
	int				is_simulation_running;
	int				start_flag;
	pthread_mutex_t	start_mutex;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	sim_mutex;
	pthread_mutex_t	*fork_mutexes;
	struct s_philo	*philo;
}	t_infos;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_timestamp;
	int				left_idx;
	int				right_idx;
	pthread_t		thread;
	t_infos			*infos;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_mutex;
}	t_philo;

int			init_infos(int argc, char **argv, t_infos *infos);
int			ft_satoi(const char *str);
int			ft_strncmp(const char *s1, const char *s2, size_t n);

void		start_simulation(t_infos *infos);
void		wait_and_start(t_philo *philo);

void		*philo_routine(void *p_data);
int			check_meal_ready(t_philo *philo);
int			lock_forks_in_order(t_philo *philo);

void		monitor_routine(t_infos *infos);

int			simulation_active(t_infos *infos);
int			smart_sleep(size_t duration_ms, t_infos*infos);
long long	get_time(void);
void		print_message(t_philo *philo, const char *msg);

void		destroy_mutexes(t_infos *info);
void		destroy_program(t_infos *info);
void		destroy_fork_mutexes(t_infos *infos, int count);

#endif