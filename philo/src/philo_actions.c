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

#include <philo.h>

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
