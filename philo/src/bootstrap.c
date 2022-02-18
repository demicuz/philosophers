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

bool	state_malloc(t_state *s, int philo_num)
{
	s->philos = malloc(sizeof(pthread_t) * philo_num);
	s->death_checkers = malloc(sizeof(pthread_t) * philo_num);
	s->philos_data = malloc(sizeof(t_philo) * philo_num);
	s->forks = malloc(sizeof(pthread_mutex_t) * philo_num * 2);
	return (s->philos && s->death_checkers && s->philos_data && s->forks);
}

void	init_philo_data(t_state *s, t_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		s->philos_data[i].all_forks = s->forks;
		s->philos_data[i].args = args;
		s->philos_data[i].index = i;
		s->philos_data[i].start = &s->start;
		s->philos_data[i].death = &s->death;
		s->philos_data[i].times_eaten = 0;
		s->philos_data[i].philos_still_eating = &s->philos_still_eating;
		s->philos_data[i].still_eating_m = &s->still_eating_m;
		i++;
	}
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

bool	init_vars(t_state *s, t_args *args)
{
	int	i;

	if (!state_malloc(s, args->philo_num))
		return (false);
	s->death = false;
	s->args = args;
	s->philos_still_eating = args->philo_num;
	i = 0;
	while (i < args->philo_num * 2)
	{
		if (pthread_mutex_init(&s->forks[i], NULL) != 0)
		{
			destroy_mutexes(s->forks, i);
			return (false);
		}
		i++;
	}
	if (pthread_mutex_init(&s->still_eating_m, NULL) != 0)
	{
		destroy_mutexes(s->forks, args->philo_num * 2);
		pthread_mutex_destroy(&s->still_eating_m);
		return (false);
	}
	init_philo_data(s, args);
	return (true);
}

void	give_forks(t_philo *philos_data, pthread_mutex_t *forks, int philo_num)
{
	int	i;

	philos_data[0].fork1_for_me = &forks[philo_num * 2 - 1];
	philos_data[0].fork1_for_neighbor = &forks[philo_num * 2 - 2];
	philos_data[0].fork2_for_me = &forks[0];
	philos_data[0].fork2_for_neighbor = &forks[1];
	i = 1;
	while (i < philo_num)
	{
		philos_data[i].fork1_for_me = &forks[i * 2];
		philos_data[i].fork1_for_neighbor = &forks[i * 2 + 1];
		philos_data[i].fork2_for_me = &forks[i * 2 - 1];
		philos_data[i].fork2_for_neighbor = &forks[i * 2 - 2];
		i++;
	}
	i = 1;
	while (i < philo_num)
	{
		pthread_mutex_lock(philos_data[i].fork1_for_me);
		pthread_mutex_lock(philos_data[i].fork2_for_me);
		i += 2;
	}
	if (philo_num % 2 == 1)
		pthread_mutex_lock(philos_data[philo_num - 1].fork1_for_me);
}
