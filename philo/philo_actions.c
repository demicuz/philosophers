/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:02:29 by psharen           #+#    #+#             */
/*   Updated: 2022/06/25 03:44:40 by psharen          ###   ########.fr       */
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

void	think(t_philo *p)
{
	lock_and_check_death(p);
	printf("%ld %d is thinking\n", time_passed(p->start) / 1000, p->index + 1);
	pthread_mutex_unlock(p->death_m);
}

// Unreadable thanks to Norminette
void	take_forks(t_philo *p)
{
	pthread_mutex_lock(p->right_fork);
	lock_and_check_death(p);
	printf("%ld %d has taken a fork\n", time_passed(p->start) / 1000,
		p->index + 1);
	pthread_mutex_unlock(p->death_m);
	pthread_mutex_lock(p->left_fork);
	lock_and_check_death(p);
	printf("%ld %d has taken a fork\n", time_passed(p->start) / 1000,
		p->index + 1);
	pthread_mutex_unlock(p->death_m);
}

void	eat(t_philo *p)
{
	unsigned long	now_micros;

	lock_and_check_death(p);
	now_micros = time_passed(p->start);
	printf("%ld %d is eating\n", now_micros / 1000, p->index + 1);
	pthread_mutex_unlock(p->death_m);
	pthread_mutex_lock(p->last_eaten_m);
	p->last_eaten = now_micros;
	pthread_mutex_unlock(p->last_eaten_m);
	usleep(p->args->time_eat * 1000);
	pthread_mutex_unlock(p->right_fork);
	pthread_mutex_unlock(p->left_fork);
}

void	take_a_nap(t_philo *p)
{
	lock_and_check_death(p);
	printf("%ld %d is sleeping\n", time_passed(p->start) / 1000, p->index + 1);
	pthread_mutex_unlock(p->death_m);
	usleep(p->args->time_sleep * 1000);
}
