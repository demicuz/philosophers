/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:04:24 by psharen           #+#    #+#             */
/*   Updated: 2022/06/22 03:20:54 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

// TODO remove unused headers
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <sys/stat.h>

#include <sys/time.h>

void	philo_routine(t_args *a, t_state *s, struct timeval *start, int index)
{
	t_philo	p;

	p.index = index; // TODO is this necessary?
	p.last_eaten_sem = s->last_eaten_sems[index];
	p.last_eaten = 0;
	p.death = false;
	int i = 0;
	while (true)
	{
		take_forks(&p, s);
		eat(&p, a, s);
		take_a_nap(&p, a, s);
		i++;
	}
	// exit(123);
}

bool	wait_simulation_end(t_args *a, t_state *s)
{
	int	wstatus;

	waitpid(-1, &wstatus, 0);
	printf("child exited with status %d\n", WEXITSTATUS(wstatus));
	kill_all(s->pids, a->philo_num);
	return (true);
}

bool	run_simulation(t_args *a, t_state *s)
{
	struct timeval	start;
	int				i;

	gettimeofday(&start, NULL);
	s->start = &start;
	i = 0;
	while (i < a->philo_num)
	{
		s->pids[i] = fork();
		if (s->pids[i] == 0)
			philo_routine(a, s, &start, i);
		else if (s->pids[i] == -1) // TODOO
		{
			kill_all(s->pids, i);
			// kill_all(s->pids, 4);
			// cleanup(a, s);
			return (false);
		}
		i++;
	}
	// TODO wait for philos here
	wait_simulation_end(a, s);
	puts("EXITING");
	return (true);
}

bool	parse_and_set_int(const char *s, int *n)
{
	int	err;

	err = 0;
	if (is_num(s))
	{
		*n = ft_atoi_safe(s, &err);
		return (!err && *n >= 0);
	}
	else
		return (false);
}

// accept 4 or 5 arguments
// must be integers equal to or greater than zero
// the code is ugly af
bool	parse_args(t_args *a, int argc, const char *argv[])
{
	a->must_eat_num = -1;
	if (argc == 5)
	{
		return (parse_and_set_int(argv[1], &a->philo_num) && \
			parse_and_set_int(argv[2], &a->time_death) && \
			parse_and_set_int(argv[3], &a->time_eat) && \
			parse_and_set_int(argv[4], &a->time_sleep));
	}
	else if (argc == 6)
	{
		return (parse_and_set_int(argv[1], &a->philo_num) && \
			parse_and_set_int(argv[2], &a->time_death) && \
			parse_and_set_int(argv[3], &a->time_eat) && \
			parse_and_set_int(argv[4], &a->time_sleep) && \
			parse_and_set_int(argv[5], &a->must_eat_num));
	}
	else
		return (false);
}

int	main(int argc, const char *argv[])
{
	t_args	args;
	t_state	state;

	if (!parse_args(&args, argc, argv))
	{
		printf(HELP_MESSAGE);
		return (EXIT_FAILURE);
	}
	if (args.must_eat_num == 0 || args.philo_num == 0)
		return (EXIT_SUCCESS);
	if (!init(&args, &state))
	{
		printf("Failed to initialize\n");
		return (EXIT_FAILURE);
	}
	if (!run_simulation(&args, &state))
	{
		printf("Failed to run the simulation\n");
		return (EXIT_FAILURE);
	}
	// exit(init_and_run(&args));
	cleanup(&args, &state);
	return (EXIT_SUCCESS);
}
