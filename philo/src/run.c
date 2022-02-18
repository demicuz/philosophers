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

bool	run_simulation(t_state *s, int philo_num)
{
	philo_routine	rt;
	int				i;

	i = 0;
	if (s->args->must_eat_num)
		rt = &routine_min_eaten;
	else
		rt = &routine;
	gettimeofday(&s->start, NULL);
	while (i < philo_num)
	{
		if (pthread_create(&s->philos[i], NULL, rt, &s->philos_data[i]) != 0)
			return (false);
		i++;
	}
	i = 0;
	while (i < philo_num)
	{
		if (pthread_create(&s->death_checkers[i], NULL, &routine_death, \
			&s->philos_data[i]) != 0)
			return (false);
		i++;
	}
	return (true);
}

void	join_all(pthread_t *philos, pthread_t *death_checkers, int philo_num)
{
	int	i;

	i = 0;
	while (i < philo_num)
	{
		pthread_join(philos[i], NULL);
		i++;
	}
	i = 0;
	while (i < philo_num)
	{
		pthread_join(death_checkers[i], NULL);
		i++;
	}
}

void	free_state(t_state *s)
{
	free(s->philos);
	free(s->death_checkers);
	free(s->philos_data);
	free(s->forks);
}

void	init_and_run(t_args *args)
{
	t_state			s;

	if (!init_vars(&s, args))
	{
		printf("Erorr while allocating memory\n");
		free_state(&s);
	}
	give_forks(s.philos_data, s.forks, args->philo_num);
	if (!run_simulation(&s, args->philo_num))
		printf("Error while creating threads\n");
	join_all(s.philos, s.death_checkers, args->philo_num);
	destroy_mutexes(s.forks, args->philo_num * 2);
	pthread_mutex_destroy(&s.still_eating_m);
	free_state(&s);
}
