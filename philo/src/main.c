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
#include <sys/time.h>

#include <philo.h>

#define PHILO_NUM 4
#define TIME_DEATH 410
#define TIME_EAT 200
#define TIME_SLEEP 200

int global = 0;

long	time_passed(struct timeval *start)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return (now.tv_sec - start->tv_sec) * 1000 + ((now.tv_usec - start->tv_usec) / 1000);
}

// void	*routine(void *philo_data)
// {
// 	t_philo 		*p;
// 	long			now_millis;

// 	p = philo_data;
// 	printf("%d %d is thinking\n", 0, p->index);
// 	while (1)
// 	{
// 		pthread_mutex_lock(p->left_fork);
// 		printf("%ld %d has taken a fork\n", time_passed(p->start), p->index);
// 		pthread_mutex_lock(p->right_fork);
// 		printf("%ld %d has taken a fork\n", time_passed(p->start), p->index);
// 		now_millis = time_passed(p->start);
// 		printf("%ld %d is eating\n", now_millis, p->index);
// 		p->last_eaten = now_millis;
// 		usleep(TIME_EAT * 1000);
// 		pthread_mutex_unlock(p->left_fork);
// 		pthread_mutex_unlock(p->right_fork);
// 		printf("%ld %d is sleeping\n", time_passed(p->start), p->index);
// 		usleep(TIME_SLEEP * 1000);
// 		printf("%ld %d is thinking\n", time_passed(p->start), p->index);
// 	}
// 	pthread_exit(NULL);
// }

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
	usleep(TIME_EAT * 1000);
}

void	release_forks(t_philo *p)
{
	pthread_mutex_unlock(p->fork1_for_neighbor);
	pthread_mutex_unlock(p->fork2_for_neighbor);
}

void	take_a_nap(t_philo *p)
{
	printf("%ld %d is sleeping\n", time_passed(p->start), p->index);
	usleep(TIME_SLEEP * 1000);
	printf("%ld %d is thinking\n", time_passed(p->start), p->index);
}

void	*routine(void *philo_data)
{
	t_philo 		*p;

	p = philo_data;
	printf("%d %d is thinking\n", 0, p->index);
	while (1)
	{
		take_forks(p);
		eat(p);
		release_forks(p);
		take_a_nap(p);
	}
}

void	*routine_death(void *philo_data)
{
	t_philo 		*p;
	struct timeval	now;
	long			now_millis;
	long			dt;

	p = philo_data;
	usleep(TIME_DEATH * 1000 + 500);
	while (1)
	{
		gettimeofday(&now, NULL);
		now_millis = (now.tv_sec - p->start->tv_sec) * 1000 + ((now.tv_usec - p->start->tv_usec) / 1000);
		dt = now_millis - p->last_eaten;
		if (dt <= TIME_DEATH)
			usleep((TIME_DEATH - dt) * 1000 + 500);
		else
		{
			printf("%ld %d died\n", time_passed(p->start), p->index);
			pthread_exit(NULL);
		}
	}
}

// TODO what if we have only one philosopher?
void	distribute_forks(t_philo *philos_data, pthread_mutex_t *forks, int n)
{
	int	i;

	philos_data[0].fork1_for_me = &forks[n - 1];
	philos_data[0].fork1_for_neighbor = &forks[n - 2];
	philos_data[0].fork2_for_me = &forks[0];
	philos_data[0].fork2_for_neighbor = &forks[1];
	i = 1;
	while (i < PHILO_NUM)
	{
		philos_data[i].fork1_for_me = &forks[i * 2];
		philos_data[i].fork1_for_neighbor = &forks[i * 2 + 1];
		philos_data[i].fork2_for_me = &forks[i * 2 - 1];
		philos_data[i].fork2_for_neighbor = &forks[i * 2 - 2];
		i++;
	}
	i = 1;
	while (i < PHILO_NUM)
	{
		pthread_mutex_lock(philos_data[i].fork1_for_me);
		pthread_mutex_lock(philos_data[i].fork2_for_me);
		i += 2;
	}
	if (PHILO_NUM % 2 == 1)
		pthread_mutex_lock(philos_data[PHILO_NUM - 1].fork1_for_me);
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


int main(int argc, const char *argv[])
{
	pthread_t *philos = malloc(sizeof(pthread_t) * PHILO_NUM);
	pthread_t *death_checkers = malloc(sizeof(pthread_t) * PHILO_NUM);
	t_philo *philos_data = malloc(sizeof(t_philo) * PHILO_NUM);
	pthread_mutex_t *forks = malloc(sizeof(pthread_mutex_t) * PHILO_NUM * 2);
	struct timeval	start;

	if (!philos || !death_checkers || !philos_data || !forks)
		exit(1);
	for (int i = 0; i < PHILO_NUM; ++i)
		pthread_mutex_init(&forks[i], NULL);

	for (int i = 0; i < PHILO_NUM; ++i)
	{
		philos_data[i].index = i;
		philos_data[i].start = &start;
		// if (i == 0)
		// {
		// 	philos_data[i].left_fork = &forks[(i + 1) % PHILO_NUM];
		// 	philos_data[i].right_fork = &forks[i];
		// }
		// else
		// {
		// 	philos_data[i].left_fork = &forks[i];
		// 	philos_data[i].right_fork = &forks[(i + 1) % PHILO_NUM];
		// }
	}
	distribute_forks(philos_data, forks, PHILO_NUM * 2);

	// Start simulation
	gettimeofday(&start, NULL);
	for (int i = 0; i < PHILO_NUM; ++i)
	{
		if (pthread_create(&philos[i], NULL, &routine, &philos_data[i]) != 0)
			exit(1);
	}
	for (int i = 0; i < PHILO_NUM; ++i)
	{
		if (pthread_create(&death_checkers[i], NULL, &routine_death, &philos_data[i]) != 0)
			exit(1);
	}
	for (int i = 0; i < PHILO_NUM; ++i)
	{
		if (pthread_join(philos[i], NULL) != 0)
			exit(1);
	}
	destroy_mutexes(forks, PHILO_NUM * 2 - 1);
	exit(0);
}
