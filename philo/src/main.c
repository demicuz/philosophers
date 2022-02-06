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

#define PHILO_NUM 199
#define TIME_DEATH 2000
#define TIME_EAT 600
#define TIME_SLEEP 60

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
	struct timeval	start;

	p = philo_data;
	gettimeofday(&start, NULL);
	while (1)
	{
		if (p->index != 0)
		{
			pthread_mutex_lock(p->left_fork);
			printf("%ld %d has taken a fork\n", time_passed(&start), p->index);
			pthread_mutex_lock(p->right_fork);
			printf("%ld %d has taken a fork\n", time_passed(&start), p->index);
		}
		else
		{
			pthread_mutex_lock(p->right_fork);
			printf("%ld %d has taken a fork\n", time_passed(&start), p->index);
			pthread_mutex_lock(p->left_fork);
			printf("%ld %d has taken a fork\n", time_passed(&start), p->index);
		}
		printf("%ld %d is eating\n", time_passed(&start), p->index);
		usleep(TIME_EAT * 1000);
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
		printf("%ld %d is sleeping\n", time_passed(&start), p->index);
		usleep(TIME_SLEEP * 1000);
		printf("%ld %d is thinking\n", time_passed(&start), p->index);
	}
	pthread_exit(NULL);
}

int main(int argc, const char *argv[])
{
	pthread_t *philos = malloc(sizeof(pthread_t) * PHILO_NUM);
	int *philo_indices = malloc(sizeof(int) * PHILO_NUM);
	t_philo *philos_data = malloc(sizeof(t_philo) * PHILO_NUM);
	pthread_mutex_t *forks = malloc(sizeof(pthread_mutex_t) * PHILO_NUM);


	if (!philos || !philos_data || !forks)
		exit(1);
	for (int i = 0; i < PHILO_NUM; ++i)
		pthread_mutex_init(&forks[i], NULL);

	for (int i = 0; i < PHILO_NUM; ++i)
	{
		philos_data[i].index = i;
		philos_data[i].left_fork = &forks[i];
		philos_data[i].right_fork = &forks[(i + 1) % PHILO_NUM];
		if (pthread_create(&philos[i], NULL, &routine, &philos_data[i]) != 0)
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
