/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:04:24 by psharen           #+#    #+#             */
/*   Updated: 2022/06/20 01:49:01 by psharen          ###   ########.fr       */
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

void	philo_action(struct timeval *start, int index)
{
	int *a = malloc(sizeof(int) * 10);
	*a = 0;
	while (true)
	{
		printf("I'm a philo number %d\n", index + 1);
		sleep(3);
	}
}

int	run_simulation(pid_t *pids, t_args *args)
{
	struct timeval	start;
	int				i;

	gettimeofday(&start, NULL);

	i = 0;
	while (i < args->philo_num)
	{
		pids[i] = fork();
		if (pids[i] == 0)
			philo_action(&start, i);
		else if (pids[i] == -1)
		{
			kill_all(pids, i);
			// free(pids);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}



int	init_and_run(t_args *args)
{

	// if (args->must_eat_num == 0 || args->philo_num == 0)
	// 	return (EXIT_SUCCESS);

	// if (!run_simulation(pids, args))
	// {
	// 	// do something here, processes should be killed
	// }
	// else
	// {
	// 	// wait for processes to exit
	// }

	// //waitpid()
	// sleep(10);
	// kill_all(pids, args->philo_num);
	// free(pids);
	// puts("All children should have exited!");
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
	puts("finished");
	cleanup(&args, &state);
	// if (!run(&args, &state))
	// {
	// 	printf("Failed to run the simulation\n");
	// 	return (EXIT_FAILURE);
	// }
	// TODO NOW split init_and_run into two functions, call them here
	// exit(init_and_run(&args));
}
