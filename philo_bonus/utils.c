/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 22:55:26 by psharen           #+#    #+#             */
/*   Updated: 2022/06/24 22:25:26 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

#include <limits.h>
#include <stdlib.h>

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

// Time passed since `start` in microseconds
unsigned long	time_passed(struct timeval *start)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec - start->tv_sec) * 1000000 +
		(now.tv_usec - start->tv_usec));
}
