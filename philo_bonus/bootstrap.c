/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bootstrap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 18:52:45 by psharen           #+#    #+#             */
/*   Updated: 2022/06/20 01:53:53 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

bool	open_philo_semaphores(t_args *a, t_state *s)
{
	int	i;

	i = 0;
	while (i < a->philo_num)
	{
		if (sem_open(s->last_eaten_sem_names[i], O_CREAT | O_EXCL, S_IRWXU, 1)
			== SEM_FAILED)
			return (false);
		i++;
	}
	return (true);
}

// Terrible, terrible code
bool	init_philo_semaphores(t_args *a, t_state *s)
{
	int		i;

	s->last_eaten_sem_names = malloc(sizeof(char *) * a->philo_num);
	s->last_eaten_sems = malloc(sizeof(sem_t *) * a->philo_num);
	if (!s->last_eaten_sem_names || !s->last_eaten_sems)
	{
		free(s->last_eaten_sem_names);
		free(s->last_eaten_sems);
		return (false);
	}
	memset(s->last_eaten_sem_names, 0, a->philo_num);
	memset(s->last_eaten_sems, 0, a->philo_num);
	i = 0;
	while (i < a->philo_num)
	{
		s->last_eaten_sem_names[i] = get_philo_sem_name(i);
		if (!s->last_eaten_sem_names[i])
			return (false);
		i++;
	}
	return (open_philo_semaphores(a, s));
}

// Init semaphores and memory for storing PIDs of philos
bool	init(t_args *a, t_state *s)
{
	sem_unlink("/21_forks");
	sem_unlink("/21_stdout");
	s->forks = sem_open("/21_forks", O_CREAT | O_EXCL, S_IRWXU, a->philo_num);
	s->stdout = sem_open("/21_stdout", O_CREAT | O_EXCL, S_IRWXU, 1);
	s->pids = malloc(sizeof(pid_t) * a->philo_num);
	if (s->forks == SEM_FAILED || s->stdout == SEM_FAILED || !s->pids
		|| !init_philo_semaphores(a, s))
	{
		cleanup(a, s);
		return (false);
	}
	return (true);
}
