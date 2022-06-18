/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:02:29 by psharen           #+#    #+#             */
/*   Updated: 2022/06/15 19:05:59 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	lock_and_check_death(t_philo *p)
{
	pthread_mutex_lock(p->death_m);
	if (*p->death)
	{
		pthread_mutex_unlock(p->death_m);
		pthread_exit(NULL);
	}
}

// Unreadable thanks to Norminette
void	take_forks(t_philo *p)
{
	lock_and_check_death(p);
	printf("%ld %d is thinking\n", time_passed(p->start), p->index + 1);
	pthread_mutex_unlock(p->death_m);
	pthread_mutex_lock(p->fork1_for_me);
	lock_and_check_death(p);
	printf("%ld %d has taken a fork\n", time_passed(p->start), p->index + 1);
	pthread_mutex_unlock(p->death_m);
	pthread_mutex_lock(p->fork2_for_me);
	lock_and_check_death(p);
	printf("%ld %d has taken a fork\n", time_passed(p->start), p->index + 1);
	pthread_mutex_unlock(p->death_m);
}

void	eat(t_philo *p)
{
	long	now_millis;

	lock_and_check_death(p);
	now_millis = time_passed(p->start);
	printf("%ld %d is eating\n", now_millis, p->index + 1);
	pthread_mutex_unlock(p->death_m);
	pthread_mutex_lock(p->last_eaten_m);
	p->last_eaten = now_millis;
	pthread_mutex_unlock(p->last_eaten_m);
	usleep(p->args->time_eat * 1000);
	pthread_mutex_unlock(p->fork1_for_neighbor);
	pthread_mutex_unlock(p->fork2_for_neighbor);
}

void	take_a_nap(t_philo *p)
{
	lock_and_check_death(p);
	printf("%ld %d is sleeping\n", time_passed(p->start), p->index + 1);
	pthread_mutex_unlock(p->death_m);
	usleep(p->args->time_sleep * 1000);
}
