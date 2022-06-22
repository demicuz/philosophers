/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 03:50:39 by psharen           #+#    #+#             */
/*   Updated: 2022/06/22 03:20:37 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// TODO can't decide whether to use last_eaten_sem or stdout

void	lock_and_check_death(t_philo *p, t_state *s)
{
	sem_wait(s->stdout);
	if (p->death)
	{
		// TODO is this needed? Maybe block other philos from pooping into
		// stdout?
		sem_post(s->stdout);
		puts("DIED");
		exit(123);
	}
}

// Unreadable thanks to Norminette
void	take_forks(t_philo *p, t_state *s)
{
	lock_and_check_death(p, s);
	printf("%ld %d is thinking\n", time_passed(s->start), p->index + 1);
	sem_post(s->stdout);

	sem_wait(s->forks);
	lock_and_check_death(p, s);
	printf("%ld %d has taken a fork\n", time_passed(s->start), p->index + 1);
	sem_post(s->stdout);

	sem_wait(s->forks);
	lock_and_check_death(p, s);
	printf("%ld %d has taken a fork\n", time_passed(s->start), p->index + 1);
	sem_post(s->stdout);
}

void	eat(t_philo *p, t_args *a, t_state *s)
{
	long	now_millis;

	lock_and_check_death(p, s);
	now_millis = time_passed(s->start);
	printf("%ld %d is eating\n", now_millis, p->index + 1);
	sem_post(s->stdout);

	sem_wait(p->last_eaten_sem);
	p->last_eaten = now_millis;
	sem_post(p->last_eaten_sem);

	usleep(a->time_eat * 1000);

	sem_post(s->forks);
	sem_post(s->forks);
}

void	take_a_nap(t_philo *p, t_args *a, t_state *s)
{
	lock_and_check_death(p, s);
	printf("%ld %d is sleeping\n", time_passed(s->start), p->index + 1);
	sem_post(s->stdout);
	usleep(a->time_sleep * 1000);
}
