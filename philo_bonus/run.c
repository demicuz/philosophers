/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 22:05:47 by psharen           #+#    #+#             */
/*   Updated: 2022/06/24 22:54:47 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

bool	wait_simulation_end(t_args *a, t_state *s)
{
	int	wstatus;

	waitpid(-1, &wstatus, 0);
	if (WEXITSTATUS(wstatus) != 255)
		printf("%ld %d died\n", time_passed(&s->start) / 1000,
			WEXITSTATUS(wstatus) + 1);
	kill_all(s->pids, a->philo_num);
	return (true);
}

bool	run_simulation(t_args *a, t_state *s)
{
	unsigned int	i;

	gettimeofday(&s->start, NULL);
	i = 0;
	while (i < a->philo_num)
	{
		s->pids[i] = fork();
		if (s->pids[i] == 0)
			philo_routine(a, s, i);
		else if (s->pids[i] == -1)
		{
			kill_all(s->pids, i);
			return (false);
		}
		i++;
	}
	wait_simulation_end(a, s);
	return (true);
}
