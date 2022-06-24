/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:04:24 by psharen           #+#    #+#             */
/*   Updated: 2022/06/24 21:39:42 by psharen          ###   ########.fr       */
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

#include <pthread.h>


void	*routine_death(void *philo_data)
{
	t_philo			*p;
	unsigned long	now_micros;
	unsigned long	dt;

	p = philo_data;
	usleep(p->args->time_death * 1000 - time_passed(p->start)
		+ CHECKER_WAIT_TIME);
	while (true)
	{
		sem_wait(p->last_eaten_sem);
		now_micros = time_passed(p->start);
		dt = now_micros - p->last_eaten;
		if (dt <= p->args->time_death * 1000)
		{
			sem_post(p->last_eaten_sem);
			// puts("death check");
			usleep(p->args->time_death * 1000 - dt + CHECKER_WAIT_TIME);
		}
		else
		{
			sem_wait(p->stdout);
			// sem_post(p->last_eaten_sem); // TODO do I really need this?
			exit(p->index);
		}
	}
}

void	set_philo_vars(t_philo *p, t_args *a, t_state *s, unsigned int index)
{
	p->index = index;
	p->last_eaten_sem = s->last_eaten_sems[index];
	p->last_eaten = 0;
	p->args = a;
	p->start = s->start;
	p->stdout = s->stdout;
}

void	init_philo(t_philo *p, t_args *a, t_state *s, unsigned int index)
{
	pthread_t	death_checker;

	set_philo_vars(p, a, s, index);
	if (pthread_create(&death_checker, NULL, routine_death, p) != 0)
	{
		sem_wait(s->stdout);
		printf("Thread creation failed!");
		exit(255);
	}
	pthread_detach(death_checker);
}

void	magic_start_delay(t_args *a, unsigned int index)
{
	if (a->philo_num % 2 == 0 && index >= a->philo_num / 2)
		usleep(PHILO_WAIT_TIME);
	else if (a->philo_num % 2 != 0 && index == a->philo_num - 1)
		usleep(a->time_eat * 1000 + PHILO_WAIT_TIME * 2);
	else if (a->philo_num % 2 != 0 && index >= a->philo_num / 2)
		usleep(PHILO_WAIT_TIME);
}

unsigned int	get_magic_wait_time(t_args *a)
{
	int	wat;

	wat = 2 * a->time_eat - a->time_sleep;
	if (a->philo_num % 2 != 0 && wat >= 0)
		return(wat * 1000 + PHILO_WAIT_TIME);
	else
		return (0);
}

void	philo_routine(t_args *a, t_state *s, unsigned int index)
{
	t_philo			p;
	int				must_eat_num;
	unsigned int	magic_wait;

	magic_wait = get_magic_wait_time(a);
	must_eat_num = a->must_eat_num;
	init_philo(&p, a, s, index);
	think(&p, s);
	magic_start_delay(a, index);
	while (true)
	{
		take_forks(&p, s);
		eat(&p, a, s);
		if (must_eat_num >= 0)
		{
			must_eat_num--;
			if (must_eat_num == 0 && ((a->philo_num % 2 == 0 && index >= a->philo_num / 2)
				|| (a->philo_num % 2 != 0 && index == a->philo_num - 1)))
				exit(255);
		}
		take_a_nap(&p, a, s);
		if (a->philo_num % 2 != 0)
			usleep(magic_wait);
		think(&p, s);
	}
}

bool	wait_simulation_end(t_args *a, t_state *s)
{
	int	wstatus;

	waitpid(-1, &wstatus, 0);
	if (WEXITSTATUS(wstatus) != 255)
		printf("%ld %d died\n", time_passed(s->start) / 1000, WEXITSTATUS(wstatus) + 1);
	kill_all(s->pids, a->philo_num);
	return (true);
}

bool	run_simulation(t_args *a, t_state *s)
{
	struct timeval	start;
	unsigned int	i;

	gettimeofday(&start, NULL);
	s->start = &start;
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

bool	parse_and_set_uint(const char *s, unsigned int *n)
{
	int	err;
	int	r;

	err = 0;
	if (is_num(s))
	{
		r = ft_atoi_safe(s, &err);
		*n = r;
		return (!err && r >= 0);
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
		return (parse_and_set_uint(argv[1], &a->philo_num) && \
			parse_and_set_uint(argv[2], &a->time_death) && \
			parse_and_set_uint(argv[3], &a->time_eat) && \
			parse_and_set_uint(argv[4], &a->time_sleep));
	}
	else if (argc == 6)
	{
		return (parse_and_set_uint(argv[1], &a->philo_num) && \
			parse_and_set_uint(argv[2], &a->time_death) && \
			parse_and_set_uint(argv[3], &a->time_eat) && \
			parse_and_set_uint(argv[4], &a->time_sleep) && \
			parse_and_set_int(argv[5], &a->must_eat_num));
	}
	else
		return (false);
}

int	main(int argc, const char *argv[])
{
	// close(1);
	t_args	args;
	t_state	state;

	if (!parse_args(&args, argc, argv))
	{
		printf(HELP_MESSAGE);
		return (EXIT_FAILURE);
	}
	if (args.must_eat_num == 0 || args.philo_num == 0)
		return (EXIT_SUCCESS);
	if (args.philo_num > 254)
	{
		printf("Just... Don't.\n");
		return (EXIT_FAILURE);
	}
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
