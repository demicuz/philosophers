/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 03:50:39 by psharen           #+#    #+#             */
/*   Updated: 2022/06/25 03:23:05 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void	think(t_philo *p, t_state *s)
{
	sem_wait(s->stdout);
	printf("%ld %d is thinking\n", time_passed(&s->start) / 1000, p->index + 1);
	sem_post(s->stdout);
}

void	take_forks(t_philo *p, t_state *s)
{
	sem_wait(s->forks);
	sem_wait(s->forks);
	sem_wait(s->stdout);
	printf("%ld %d has taken a fork\n", time_passed(&s->start) / 1000,
		p->index + 1);
	printf("%ld %d has taken a fork\n", time_passed(&s->start) / 1000,
		p->index + 1);
	sem_post(s->stdout);
}

void	eat(t_philo *p, t_args *a, t_state *s)
{
	unsigned long	now_micros;

	sem_wait(s->stdout);
	now_micros = time_passed(&s->start);
	printf("%ld %d is eating\n", now_micros / 1000, p->index + 1);
	sem_post(s->stdout);
	sem_wait(p->last_eaten_sem);
	p->last_eaten = now_micros;
	sem_post(p->last_eaten_sem);
	usleep(a->time_eat * 1000);
	sem_post(s->forks);
	sem_post(s->forks);
}

void	take_a_nap(t_philo *p, t_args *a, t_state *s)
{
	sem_wait(s->stdout);
	printf("%ld %d is sleeping\n", time_passed(&s->start) / 1000, p->index + 1);
	sem_post(s->stdout);
	usleep(a->time_sleep * 1000);
}
