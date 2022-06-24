/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:02:33 by psharen           #+#    #+#             */
/*   Updated: 2022/06/24 06:13:51 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	*routine(void *philo_data)
{
	t_philo	*p;

	p = philo_data;
	while (true)
	{
		take_forks(p);
		eat(p);
		take_a_nap(p);
	}
}

void	terminate_if_all_eaten(t_philo *p)
{
	pthread_mutex_lock(p->still_eating_m);
	(*p->philos_still_eating)--;
	if (*p->philos_still_eating != 0)
	{
		pthread_mutex_unlock(p->still_eating_m);
		return ;
	}
	pthread_mutex_unlock(p->still_eating_m);
	pthread_mutex_lock(p->death_m);
	*p->death = true;
	pthread_mutex_unlock(p->death_m);
	unlock_all_mutexes(p->all_forks, p->args->philo_num * 2);
	pthread_exit(NULL);
}

void	*routine_min_eaten(void *philo_data)
{
	t_philo	*p;

	p = philo_data;
	while (true)
	{
		take_forks(p);
		eat(p);
		p->times_eaten++;
		if (p->times_eaten == p->args->must_eat_num)
			terminate_if_all_eaten(p);
		take_a_nap(p);
	}
}

void	die(t_philo *p, long now_micros)
{
	pthread_mutex_lock(p->death_m);
	if (!*p->death)
		printf("%ld %d died\n", now_micros / 1000, p->index + 1);
	*p->death = true;
	pthread_mutex_unlock(p->death_m);
	pthread_mutex_unlock(p->last_eaten_m);
	unlock_all_mutexes(p->all_forks, p->args->philo_num * 2);
	pthread_exit(NULL);
}

// It would be much easier if we were allowed to use `close()`. Then we
// wouldn't need a mutex for `printf()`. Just `close()` stdout and that's it.
// `printf()` is thread-safe anyway.
void	*routine_death(void *philo_data)
{
	t_philo			*p;
	unsigned long	now_micros;
	unsigned long	dt;

	p = philo_data;
	usleep(p->args->time_death * 1000 - time_passed(p->start) + MIN_WAIT_TIME);
	while (true)
	{
		pthread_mutex_lock(p->last_eaten_m);
		now_micros = time_passed(p->start);
		dt = now_micros - p->last_eaten;
		if (dt <= p->args->time_death * 1000)
		{
			pthread_mutex_unlock(p->last_eaten_m);
			usleep(p->args->time_death * 1000 - dt + MIN_WAIT_TIME);
		}
		else
			die(p, now_micros / 1000);
	}
}
