/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bootstrap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:02:04 by psharen           #+#    #+#             */
/*   Updated: 2022/06/25 03:57:39 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

bool	state_malloc(t_state *s, int philo_num)
{
	s->philos = malloc(sizeof(pthread_t) * philo_num);
	s->death_checkers = malloc(sizeof(pthread_t) * philo_num);
	s->philos_data = malloc(sizeof(t_philo) * philo_num);
	s->forks = malloc(sizeof(pthread_mutex_t) * philo_num);
	s->last_eaten = malloc(sizeof(pthread_mutex_t) * philo_num);
	return (s->philos && s->death_checkers && s->philos_data && s->forks
		&& s->last_eaten);
}

void	init_philo_data(t_state *s, t_args *args)
{
	unsigned int	i;

	i = 0;
	while (i < args->philo_num)
	{
		s->philos_data[i].all_forks = s->forks;
		s->philos_data[i].args = args;
		s->philos_data[i].index = i;
		s->philos_data[i].start = &s->start;
		s->philos_data[i].death = &s->death;
		s->philos_data[i].times_eaten = 0;
		s->philos_data[i].last_eaten = 0;
		s->philos_data[i].philos_still_eating = &s->philos_still_eating;
		s->philos_data[i].still_eating_m = &s->still_eating_m;
		s->philos_data[i].death_m = &s->death_m;
		s->philos_data[i].last_eaten_m = &s->last_eaten[i];
		i++;
	}
}

void	destroy_mutexes(pthread_mutex_t *mutexes, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&mutexes[i]);
		i++;
	}
}

bool	init_vars(t_state *s, t_args *args)
{
	if (!state_malloc(s, args->philo_num) || !init_mutexes(s, args))
		return (false);
	s->death = false;
	s->args = args;
	s->philos_still_eating = args->philo_num;
	init_philo_data(s, args);
	return (true);
}

// TODO I don't think I need to switch forks for one philo to prevent a
// deadlock. With my initial configuration of open/closed forks it's impossible
// at the start, and during the simulation it's impossible too, though I'm not
// completely sure. So I left it the way it was, it won't harm anyway.
// UPD: now I don't open/close mutexes, just stupid wait time hack. Still
// switching forks is redundant, but ?????????? ????????.
void	give_forks(t_philo *philos_data, pthread_mutex_t *forks, int philo_num)
{
	int	i;

	philos_data[0].right_fork = &forks[0];
	philos_data[0].left_fork = &forks[philo_num - 1];
	i = 1;
	while (i < philo_num)
	{
		philos_data[i].right_fork = &forks[i - 1];
		philos_data[i].left_fork = &forks[i];
		i++;
	}
}
