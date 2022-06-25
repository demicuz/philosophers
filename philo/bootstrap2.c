/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bootstrap2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 21:27:43 by psharen           #+#    #+#             */
/*   Updated: 2022/06/25 03:25:56 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

bool	init_eat_death_mutexes(t_state *s, t_args *args)
{
	if (pthread_mutex_init(&s->still_eating_m, NULL) != 0 || \
		pthread_mutex_init(&s->death_m, NULL) != 0)
	{
		destroy_mutexes(s->forks, args->philo_num);
		pthread_mutex_destroy(&s->still_eating_m);
		pthread_mutex_destroy(&s->death_m);
		return (false);
	}
	return (true);
}

bool	init_mutexes(t_state *s, t_args *args)
{
	unsigned int	i;

	i = 0;
	while (i < args->philo_num)
	{
		if (pthread_mutex_init(&s->forks[i], NULL) != 0)
		{
			destroy_mutexes(s->forks, i);
			return (false);
		}
		i++;
	}
	i = 0;
	while (i < args->philo_num)
	{
		if (pthread_mutex_init(&s->last_eaten[i], NULL) != 0)
		{
			destroy_mutexes(s->forks, args->philo_num);
			destroy_mutexes(s->last_eaten, i);
			return (false);
		}
		i++;
	}
	return (init_eat_death_mutexes(s, args));
}
