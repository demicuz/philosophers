/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:02:40 by psharen           #+#    #+#             */
/*   Updated: 2022/06/16 22:56:38 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <limits.h>

int	is_num(const char *s)
{
	if (!s || !*s)
		return (0);
	if (*s == '+' || *s == '-')
		s++;
	if (!*s)
		return (0);
	while (*s && *s >= '0' && *s <= '9')
		s++;
	return (*s == '\0');
}

int	ft_atoi_safe(const char *str, int *err)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result *= 10;
		result += (*str - '0') * sign;
		if (result > INT_MAX || result < INT_MIN)
		{
			*err = 1;
			return (0);
		}
		str++;
	}
	return (result);
}

// Time passed since `start` in milliseconds
long	time_passed(struct timeval *start)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec - start->tv_sec) * 1000 + \
		((now.tv_usec - start->tv_usec) / 1000));
}

// TODO Unlocking an unlocked mutex is UB. But it shouldn't crash the program.
// The mutex state can be broken, or the mutex functionality itself. But at the
// moment of calling this function (when we're unlocking all the forks) we don't
// care about unlocked mutexes, only about the locked ones.
void	unlock_all_mutexes(pthread_mutex_t *mutexes, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_unlock(&mutexes[i]);
		i++;
	}
}
