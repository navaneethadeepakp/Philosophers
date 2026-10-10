/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naperisw <naperisw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 18:25:53 by naperisw          #+#    #+#             */
/*   Updated: 2026/01/27 12:04:28 by naperisw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	parse_number(const char *str, int i, int sign, long long *out)
{
	long long	result;

	result = 0;
	if (str[i] < '0' || str[i] > '9')
		return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i++] - '0');
		if ((sign == 1 && result > INT_MAX)
			|| (sign == -1 && (-result) < INT_MIN))
			return (-1);
	}
	if (str[i] != '\0')
		return (-1);
	*out = result * sign;
	return (0);
}

int	ft_satoi(const char *str)
{
	int			i;
	int			sign;
	long long	val;

	if (!str || !str[0])
		return (-1);
	i = 0;
	sign = 1;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (parse_number(str, i, sign, &val) != 0)
		return (-1);
	return ((int)val);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *) s1;
	str2 = (unsigned char *) s2;
	i = 0;
	while ((str1[i] != '\0' || str2[i] != '\0') && i < n)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}

int	smart_sleep(size_t duration_ms, t_infos *infos)
{
	size_t	start_time;

	start_time = get_time();
	while ((get_time() - start_time) < duration_ms)
	{
		if (!simulation_active(infos))
			return (0);
		usleep(500);
	}
	return (1);
}

int	check_meal_ready(t_philo *philo)
{
	int	can_eat;

	pthread_mutex_lock(&philo->meal_mutex);
	can_eat = (philo->infos->meals_required == -1
			|| philo->meals_eaten < philo->infos->meals_required);
	pthread_mutex_unlock(&philo->meal_mutex);
	return (can_eat);
}
