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

#define PHILO_NUM 5
#define TIME_DEATH 800
#define TIME_EAT 200
#define TIME_SLEEP 200

int global = 0;

long	time_passed(struct timeval *start)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return (now.tv_sec - start->tv_sec) * 1000 + ((now.tv_usec - start->tv_usec) / 1000);
}

void	*routine(void *philo_data)
{
	t_philo 		*p;
	long			now_millis;

	p = philo_data;
	printf("%d %d is thinking\n", 0, p->index);
	while (1)
	{
		// p->last_eaten =
		pthread_mutex_lock(p->left_fork);
		printf("%ld %d has taken a fork\n", time_passed(p->start), p->index);
		pthread_mutex_lock(p->right_fork);
		printf("%ld %d has taken a fork\n", time_passed(p->start), p->index);
		now_millis = time_passed(p->start);
		printf("%ld %d is eating\n", now_millis, p->index);
		p->last_eaten = now_millis;
		usleep(TIME_EAT * 1000);
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
		printf("%ld %d is sleeping\n", time_passed(p->start), p->index);
		usleep(TIME_SLEEP * 1000);
		printf("%ld %d is thinking\n", time_passed(p->start), p->index);
	}
	pthread_exit(NULL);
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
		if (dt > TIME_DEATH)
		{
			printf("%ld %d died\n", time_passed(p->start), p->index);
			pthread_exit(NULL);
		}
		else
			usleep((TIME_DEATH - dt) * 1000 + 500);
	}
	pthread_exit(NULL);
}

int main(int argc, const char *argv[])
{
	pthread_t *philos = malloc(sizeof(pthread_t) * PHILO_NUM);
	pthread_t *death_checkers = malloc(sizeof(pthread_t) * PHILO_NUM);
	t_philo *philos_data = malloc(sizeof(t_philo) * PHILO_NUM);
	pthread_mutex_t *forks = malloc(sizeof(pthread_mutex_t) * PHILO_NUM);
	struct timeval	start;

	if (!philos || !death_checkers || !philos_data || !forks)
		exit(1);
	for (int i = 0; i < PHILO_NUM; ++i)
		pthread_mutex_init(&forks[i], NULL);

	for (int i = 0; i < PHILO_NUM; ++i)
	{
		philos_data[i].index = i;
		if (i == 0)
		{
			philos_data[i].left_fork = &forks[(i + 1) % PHILO_NUM];
			philos_data[i].right_fork = &forks[i];
		}
		else
		{
			philos_data[i].left_fork = &forks[i];
			philos_data[i].right_fork = &forks[(i + 1) % PHILO_NUM];
		}
		philos_data[i].start = &start;
	}

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
	printf("global is %d\n", global);
	exit(0);
}
