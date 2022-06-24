/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 03:50:39 by psharen           #+#    #+#             */
/*   Updated: 2022/06/24 22:20:24 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// void	lock_and_check_death(t_philo *p, t_state *s)
// {
// 	sem_wait(s->stdout);
// 	if (p->died) // TODO maybe I shouldn't even check this
// 	{
// 		printf("%d DIED in lock_and_check_death()\n", p->index); // TODO remove this
// 		// TODO is this needed? Maybe block other philos from pooping into
// 		// stdout?
// 		// sem_post(s->stdout);
// 		exit(p->index);
// 	}
// }

void	think(t_philo *p, t_state *s)
{
	sem_wait(s->stdout);
	printf("%ld %d is thinking\n", time_passed(s->start) / 1000, p->index + 1);
	sem_post(s->stdout);
}

// Unreadable thanks to Norminette
void	take_forks(t_philo *p, t_state *s)
{
	// lock_and_check_death(p, s);

	// TODO room for optimization - take two forks at once, then check death
	// and print two messages
	sem_wait(s->forks);
	// lock_and_check_death(p, s);
	sem_wait(s->stdout);
	printf("%ld %d has taken a fork\n", time_passed(s->start) / 1000, p->index + 1);
	sem_post(s->stdout);

	sem_wait(s->forks);
	// lock_and_check_death(p, s);
	sem_wait(s->stdout);
	printf("%ld %d has taken a fork\n", time_passed(s->start) / 1000, p->index + 1);
	sem_post(s->stdout);
}

void	eat(t_philo *p, t_args *a, t_state *s)
{
	// unsigned long	now_millis;
	unsigned long	now_micros;

	// lock_and_check_death(p, s);
	sem_wait(s->stdout);
	now_micros = time_passed(s->start);
	// TODO rounding error. The only difference is that I divide by 1000
	// a little bit later. WAT?
	printf("%ld %d is eating\n", now_micros / 1000, p->index + 1);
	// printf("%ld %d is eating\n", time_passed(s->start), p->index + 1);
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
	// lock_and_check_death(p, s);
	sem_wait(s->stdout);
	printf("%ld %d is sleeping\n", time_passed(s->start) / 1000, p->index + 1);
	sem_post(s->stdout);
	usleep(a->time_sleep * 1000);
}
