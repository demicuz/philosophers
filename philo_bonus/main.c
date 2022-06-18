/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:04:24 by psharen           #+#    #+#             */
/*   Updated: 2022/06/18 13:59:36 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

// TODO remove unused headers
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/wait.h>

#include <semaphore.h>

#include <signal.h>

void	philo_action()
{
	int *a = malloc(sizeof(int) * 10);
	*a = 0;
	while (true)
	{
		puts("ima philo");
		sleep(1);
	}
}

int	init_and_run(t_args *args)
{
	// int	*philo_pids = malloc(sizeof(int) * args->philo_num);
	// int i = 0;
	pid_t pid = fork();
	if (pid == 0)
		philo_action();
	sleep(3);
	kill(pid, SIGTERM);
	puts("I'm not dead");
	// wait(NULL);
	return (0);
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
	a->must_eat_num = 0;
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

	if (!parse_args(&args, argc, argv))
	{
		printf(HELP_MESSAGE);
		return (EXIT_FAILURE);
	}
	return (init_and_run(&args));
}
