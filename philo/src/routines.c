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
