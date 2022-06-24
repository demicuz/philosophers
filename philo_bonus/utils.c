/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 22:55:26 by psharen           #+#    #+#             */
/*   Updated: 2022/06/24 21:38:00 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

#include <stdio.h> // TODO remove this

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
// unsigned long	time_passed(struct timeval *start)
// {
// 	struct timeval	now;

// 	gettimeofday(&now, NULL);
// 	printf("Time: %ld\n", (now.tv_sec - start->tv_sec) * 1000000 + ((now.tv_usec - start->tv_usec)));
// 	printf("Sec: %ld, usec: %ld\n", (now.tv_sec - start->tv_sec), ((now.tv_usec - start->tv_usec)));
// 	printf("Returned: %ld\n", (now.tv_sec - start->tv_sec) * 1000 + ((now.tv_usec - start->tv_usec) / 1000));
// 	return ((now.tv_sec - start->tv_sec) * 1000 +
// 		((now.tv_usec - start->tv_usec) / 1000));
// }

// Time passed since `start` in microseconds
unsigned long	time_passed(struct timeval *start)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec - start->tv_sec) * 1000000 +
		(now.tv_usec - start->tv_usec));
}

void	kill_all(pid_t *pids, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pids[i] != 0)
			kill(pids[i], SIGTERM);
		i++;
	}
}

int	ft_count_digits(int n)
{
	int	count;

	if (n == 0)
		return (1);
	count = 0;
	while (n)
	{
		n /= 10;
		count++;
	}
	return (count);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*d;
	char	*s;

	if (!dest && !src)
		return (NULL);
	else if (dest != src)
	{
		d = dest;
		s = (char *)src;
		while (n)
		{
			*d = *s;
			d++;
			s++;
			n--;
		}
	}
	return (dest);
}

// Don't even try to understand this mess!
char	*get_philo_sem_name(int index)
{
	int		n_digits;
	int		i;
	char	*result;

	n_digits = ft_count_digits(index);
	result = malloc(sizeof(char) * (n_digits + 1 + 3));
	if (!result)
		return (NULL);
	ft_memcpy(result, "ph_", 3);
	i = n_digits - 1 + 3;
	result[i + 1] = '\0';
	while (i >= 3)
	{
		result[i] = (index % 10) + '0';
		index /= 10;
		i--;
	}
	return (result);
}

// void	free_names(char **last_eaten_sem_names, int n)
// {
// 	int	i;

// 	i = 0;
// 	while (i < n)
// 	{
// 		free(last_eaten_sem_names[i]);
// 		i++;
// 	}
// }

// void	unlink_sems(sem_t **sems, char **names, int n)
// {
// 	int	i;

// 	i = 0;
// 	while (i < n)
// 	{
// 		sem_close(sems[i]);
// 		sem_unlink(names[i]);
// 		i++;
// 	}
// }

void	cleanup(t_args *a, t_state *s)
{
	unsigned int	i;

	sem_unlink("/21_forks");
	sem_unlink("/21_stdout");
	free(s->pids);
	i = 0;
	if (s->last_eaten_sem_names && s->last_eaten_sems)
	{
		while (i < a->philo_num && s->last_eaten_sem_names[i])
		{
			sem_close(s->last_eaten_sems[i]);
			sem_unlink(s->last_eaten_sem_names[i]);
			free(s->last_eaten_sem_names[i]);
			i++;
		}
	}
	free(s->last_eaten_sem_names);
	free(s->last_eaten_sems);
	// TODO made this just in case, I should assure I don't call cleanup()
	// twice and remove these lines
	s->pids = NULL;
	s->last_eaten_sem_names = NULL;
	s->last_eaten_sems = NULL;

	// unlink_sems(s->last_eaten_sems, s->last_eaten_sem_names, a->philo_num);
	// free_names(s->last_eaten_sem_names, a->philo_num);
}
