/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 22:20:03 by psharen           #+#    #+#             */
/*   Updated: 2022/06/26 08:53:45 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

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

// Top naming in action. Don't try to understand this mess. This is a hack.
unsigned int	get_magic_wait_time(t_args *a)
{
	int	wat;

	if (a->philo_num % 2 == 1)
		wat = 2 * a->time_eat - a->time_sleep;
	else
		wat = a->time_eat - a->time_sleep;
	if (wat >= 0)
		return (wat * 1000 + PHILO_WAIT_TIME);
	else
		return (0);
}

// Setting pointers to NULL just in case. I don't call it twice anyway.
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
	s->pids = NULL;
	s->last_eaten_sem_names = NULL;
	s->last_eaten_sems = NULL;
}
