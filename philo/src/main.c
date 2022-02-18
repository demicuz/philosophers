/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2000/01/01 20:20:20 by psharen           #+#    #+#             */
/*   Updated: 2000/01/01 20:20:20 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <philo.h>

long	time_passed(struct timeval *start)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec - start->tv_sec) * 1000 + \
		((now.tv_usec - start->tv_usec) / 1000));
}

void	take_forks(t_philo *p)
{
	pthread_mutex_lock(p->fork1_for_me);
	printf("%ld %d has taken a fork\n", time_passed(p->start), p->index);
	pthread_mutex_lock(p->fork2_for_me);
	printf("%ld %d has taken a fork\n", time_passed(p->start), p->index);
}

void	eat(t_philo *p)
{
	long	now_millis;

	now_millis = time_passed(p->start);
	printf("%ld %d is eating\n", now_millis, p->index);
	p->last_eaten = now_millis;
	usleep(p->args->time_eat * 1000);
}

void	release_forks(t_philo *p)
{
	pthread_mutex_unlock(p->fork1_for_neighbor);
	pthread_mutex_unlock(p->fork2_for_neighbor);
}

void	take_a_nap(t_philo *p)
{
	printf("%ld %d is sleeping\n", time_passed(p->start), p->index);
	usleep(p->args->time_sleep * 1000);
	printf("%ld %d is thinking\n", time_passed(p->start), p->index);
}

void	unlock_all_forks(pthread_mutex_t *forks, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_unlock(&forks[i]);
		i++;
	}
}

void	*routine(void *philo_data)
{
	t_philo	*p;

	p = philo_data;
	printf("%d %d is thinking\n", 0, p->index);
	while (!*p->death)
	{
		take_forks(p);
		if (*p->death)
			pthread_exit(NULL);
		eat(p);
		release_forks(p);
		if (*p->death)
			pthread_exit(NULL);
		take_a_nap(p);
	}
	pthread_exit(NULL);
}

void	terminate_if_all_eaten(t_philo *p)
{
	if (*p->philos_still_eating != 0)
		return ;
	close(1);
	*p->death = true;
	unlock_all_forks(p->all_forks, p->args->philo_num * 2);
	pthread_exit(NULL);
}

void	*routine_min_eaten(void *philo_data)
{
	t_philo	*p;

	p = philo_data;
	printf("%d %d is thinking\n", 0, p->index);
	while (!*p->death)
	{
		take_forks(p);
		if (*p->death)
			pthread_exit(NULL);
		eat(p);
		p->times_eaten++;
		if (p->times_eaten == p->args->must_eat_num)
		{
			pthread_mutex_lock(p->still_eating_m);
			(*p->philos_still_eating)--;
			pthread_mutex_unlock(p->still_eating_m);
		}
		terminate_if_all_eaten(p);
		release_forks(p);
		if (*p->death)
			pthread_exit(NULL);
		take_a_nap(p);
	}
	pthread_exit(NULL);
}

void	*routine_death(void *philo_data)
{
	t_philo			*p;
	struct timeval	now;
	long			now_millis;
	long			dt;

	p = philo_data;
	usleep(p->args->time_death * 1000 + 500);
	while (!*p->death)
	{
		gettimeofday(&now, NULL);
		now_millis = (now.tv_sec - p->start->tv_sec) * 1000 + \
			((now.tv_usec - p->start->tv_usec) / 1000);
		dt = now_millis - p->last_eaten;
		if (dt <= p->args->time_death)
			usleep((p->args->time_death - dt) * 1000 + 500);
		else
		{
			printf("%ld %d died\n", now_millis, p->index);
			close(1);
			*p->death = true;
			unlock_all_forks(p->all_forks, p->args->philo_num * 2);
			pthread_exit(NULL);
		}
	}
	pthread_exit(NULL);
}

void	give_forks(t_philo *philos_data, pthread_mutex_t *forks, int philo_num)
{
	int	i;

	philos_data[0].fork1_for_me = &forks[philo_num * 2 - 1];
	philos_data[0].fork1_for_neighbor = &forks[philo_num * 2 - 2];
	philos_data[0].fork2_for_me = &forks[0];
	philos_data[0].fork2_for_neighbor = &forks[1];
	i = 1;
	while (i < philo_num)
	{
		philos_data[i].fork1_for_me = &forks[i * 2];
		philos_data[i].fork1_for_neighbor = &forks[i * 2 + 1];
		philos_data[i].fork2_for_me = &forks[i * 2 - 1];
		philos_data[i].fork2_for_neighbor = &forks[i * 2 - 2];
		i++;
	}
	i = 1;
	while (i < philo_num)
	{
		pthread_mutex_lock(philos_data[i].fork1_for_me);
		pthread_mutex_lock(philos_data[i].fork2_for_me);
		i += 2;
	}
	if (philo_num % 2 == 1)
		pthread_mutex_lock(philos_data[philo_num - 1].fork1_for_me);
}

void	destroy_mutexes(pthread_mutex_t *forks, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

void	join_all(pthread_t *philos, pthread_t *death_checkers, int philo_num)
{
	int	i;

	i = 0;
	while (i < philo_num)
	{
		pthread_join(philos[i], NULL);
		i++;
	}
	i = 0;
	while (i < philo_num)
	{
		pthread_join(death_checkers[i], NULL);
		i++;
	}
}

bool	run_simulation(t_state *s, int philo_num)
{
	philo_routine	rt;
	int				i;

	i = 0;
	if (s->args->must_eat_num)
		rt = &routine_min_eaten;
	else
		rt = &routine;
	gettimeofday(&s->start, NULL);
	while (i < philo_num)
	{
		if (pthread_create(&s->philos[i], NULL, rt, &s->philos_data[i]) != 0)
			return (false);
		i++;
	}
	i = 0;
	while (i < philo_num)
	{
		if (pthread_create(&s->death_checkers[i], NULL, &routine_death, \
			&s->philos_data[i]) != 0)
			return (false);
		i++;
	}
	return (true);
}

bool	state_malloc(t_state *s, int philo_num)
{
	s->philos = malloc(sizeof(pthread_t) * philo_num);
	s->death_checkers = malloc(sizeof(pthread_t) * philo_num);
	s->philos_data = malloc(sizeof(t_philo) * philo_num);
	s->forks = malloc(sizeof(pthread_mutex_t) * philo_num * 2);
	return (s->philos && s->death_checkers && s->philos_data && s->forks);
}

void	init_philo_data(t_state *s, t_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		s->philos_data[i].all_forks = s->forks;
		s->philos_data[i].args = args;
		s->philos_data[i].index = i;
		s->philos_data[i].start = &s->start;
		s->philos_data[i].death = &s->death;
		s->philos_data[i].times_eaten = 0;
		s->philos_data[i].philos_still_eating = &s->philos_still_eating;
		s->philos_data[i].still_eating_m = &s->still_eating_m;
		i++;
	}
}

bool	init_vars(t_state *s, t_args *args)
{
	int	i;

	if (!state_malloc(s, args->philo_num))
		return (false);
	s->death = false;
	s->args = args;
	s->philos_still_eating = args->philo_num;
	i = 0;
	while (i < args->philo_num * 2)
	{
		if (pthread_mutex_init(&s->forks[i], NULL) != 0)
		{
			destroy_mutexes(s->forks, i);
			return (false);
		}
		i++;
	}
	if (pthread_mutex_init(&s->still_eating_m, NULL) != 0)
	{
		destroy_mutexes(s->forks, args->philo_num * 2);
		pthread_mutex_destroy(&s->still_eating_m);
		return (false);
	}
	init_philo_data(s, args);
	return (true);
}

void	free_state(t_state *s)
{
	free(s->philos);
	free(s->death_checkers);
	free(s->philos_data);
	free(s->forks);
}

void	init_and_run(t_args *args)
{
	t_state			s;

	if (!init_vars(&s, args))
	{
		printf("Erorr while allocating memory\n");
		free_state(&s);
	}
	give_forks(s.philos_data, s.forks, args->philo_num);
	if (!run_simulation(&s, args->philo_num))
		printf("Error while creating threads\n");
	join_all(s.philos, s.death_checkers, args->philo_num);
	destroy_mutexes(s.forks, args->philo_num * 2);
	pthread_mutex_destroy(&s.still_eating_m);
	free_state(&s);
}

bool	parse_and_set_int(const char *s, int *n)
{
	int	err;

	if (is_num(s))
	{
		*n = ft_atoi_safe(s, &err);
		return (!err && *n > 0);
	}
	else
		return (false);
}

// accept 4 or 5 arguments
// must be integers greater than zero
bool	parse_args(t_args *a, int argc, const char *argv[])
{
	a->must_eat_num = 0;
	if (argc == 5 || argc == 6)
	{
		return (parse_and_set_int(argv[1], &a->philo_num) && \
			parse_and_set_int(argv[2], &a->time_death) && \
			parse_and_set_int(argv[3], &a->time_eat) && \
			parse_and_set_int(argv[4], &a->time_sleep) && \
			(argc != 6 || parse_and_set_int(argv[5], &a->must_eat_num)));
	}
	else
		return (false);
}

int	main(int argc, const char *argv[])
{
	t_args	args;

	if (!parse_args(&args, argc, argv))
	{
		printf("Bad arguments\n");
		return (0);
	}
	init_and_run(&args);
	return (0);
}
