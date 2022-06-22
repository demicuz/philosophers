/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:04:24 by psharen           #+#    #+#             */
/*   Updated: 2022/06/22 21:57:54 by psharen          ###   ########.fr       */
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

void	die(t_philo *p, long now_millis)
{
	// pthread_mutex_lock(p->death_m);
	sem_wait(p->stdout);
	// if (!p->died)
		// printf("%ld %d died\n", now_millis, p->index + 1);
	p->died = true;
	// pthread_mutex_unlock(p->death_m);
	sem_post(p->stdout);
	sem_post(p->last_eaten_sem);
	printf("%d DIED in die()\n", p->index); // TODO
	// pthread_exit(NULL);
	// exit(p->index); // TODO does this kill the philo thread?
	exit(255);
}

// TODO reduce 500ms for additional waiting. Philos live with 4 400 200 200,
// though they shouldn't.
void	*routine_death(void *philo_data)
{
	t_philo			*p;
	long			now_millis;
	long			dt;

	p = philo_data;
	// usleep(p->args->time_death * 1000 + 500);
	usleep(p->args->time_death * 1000);
	while (true)
	{
		// pthread_mutex_lock(p->last_eaten_m);
		sem_wait(p->last_eaten_sem);
		now_millis = time_passed(p->start);
		dt = now_millis - p->last_eaten;
		if (dt <= p->args->time_death)
		{
			// pthread_mutex_unlock(p->last_eaten_m);
			sem_post(p->last_eaten_sem);
			// usleep((p->args->time_death - dt) * 1000 + 500);
			usleep((p->args->time_death - dt) * 1000 + 50);
		}
		else
			die(p, now_millis);
	}
}

void	philo_routine(t_args *a, t_state *s, struct timeval *start, int index)
{
	t_philo		p;
	pthread_t	deatch_checker;

	p.index = index; // TODO is this necessary?
	p.last_eaten_sem = s->last_eaten_sems[index];
	p.last_eaten = 0; // 0 is the beginning of the simulation
	p.died = false;
	p.args = a;
	p.start = start;
	p.stdout = s->stdout;
	// create a death checker
	if (pthread_create(&deatch_checker, NULL, routine_death, &p) != 0)
	{
		puts("THREAD CREATION FAILED");
		exit(255);
		// return (false);
	}
	pthread_detach(deatch_checker);
	while (true)
	{
		take_forks(&p, s);
		eat(&p, a, s);
		take_a_nap(&p, a, s);
	}
}

bool	wait_simulation_end(t_args *a, t_state *s)
{
	int	wstatus;

	waitpid(-1, &wstatus, 0);
	sem_wait(s->stdout); // TODO hangs sometimes
	// printf("child exited with status %d\n", WEXITSTATUS(wstatus));
	printf("%ld %d died\n", time_passed(s->start), WEXITSTATUS(wstatus) + 1);
	kill_all(s->pids, a->philo_num);
	sem_post(s->stdout);
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
		else if (s->pids[i] == -1)
		{
			kill_all(s->pids, i);
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
	if (args.philo_num > 255)
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
