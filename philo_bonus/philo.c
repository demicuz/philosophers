/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 22:09:33 by psharen           #+#    #+#             */
/*   Updated: 2022/06/26 06:01:24 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

void	die(t_philo *p)
{
	sem_wait(p->stdout);
	exit(p->index);
}

void	*routine_death(void *philo_data)
{
	t_philo			*p;
	unsigned long	now_micros;
	unsigned long	dt;

	p = philo_data;
	if (p->args->time_death == 0)
		die(p);
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
			usleep(p->args->time_death * 1000 - dt + CHECKER_WAIT_TIME);
		}
		else
			die(p);
	}
}

void	set_philo_vars(t_philo *p, t_args *a, t_state *s, unsigned int index)
{
	p->index = index;
	p->last_eaten_sem = s->last_eaten_sems[index];
	p->last_eaten = 0;
	p->args = a;
	p->start = &s->start;
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

void	philo_routine(t_args *a, t_state *s, unsigned int index)
{
	t_philo			p;
	unsigned int	magic_wait;

	magic_wait = get_magic_wait_time(a);
	init_philo(&p, a, s, index);
	think(&p, s);
	while (true)
	{
		take_forks(&p, s);
		eat(&p, a, s);
		if (a->must_eat_num > 0)
		{
			a->must_eat_num--;
			if (a->must_eat_num == 0
				&& ((a->philo_num % 2 == 0 && index >= a->philo_num / 2)
					|| (a->philo_num % 2 != 0 && index == a->philo_num - 1)))
				exit(255);
		}
		take_a_nap(&p, a, s);
		think(&p, s);
		if (a->philo_num % 2 != 0)
			usleep(magic_wait);
	}
}
