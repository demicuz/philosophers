/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:02:36 by psharen           #+#    #+#             */
/*   Updated: 2022/06/25 03:34:39 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

bool	run_simulation(t_state *s, int philo_num)
{
	t_philo_routine	rt;
	int				i;

	i = 0;
	if (s->args->must_eat_num != -1)
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

void	cleanup(t_state *s)
{
	free(s->philos);
	free(s->death_checkers);
	free(s->philos_data);
	free(s->forks);
	free(s->last_eaten);
}

int	init_and_run(t_args *args)
{
	t_state	s;
	int		exit_code;

	if (!init_vars(&s, args))
	{
		printf("Error while allocating memory or initializing mutexes\n");
		cleanup(&s);
		return (EXIT_FAILURE);
	}
	give_forks(s.philos_data, s.forks, args->philo_num);
	exit_code = EXIT_SUCCESS;
	if (!run_simulation(&s, args->philo_num))
	{
		printf("Error while creating threads\n");
		exit_code = EXIT_FAILURE;
	}
	join_all(s.philos, s.death_checkers, args->philo_num);
	destroy_mutexes(s.forks, args->philo_num);
	destroy_mutexes(s.last_eaten, args->philo_num);
	pthread_mutex_destroy(&s.still_eating_m);
	pthread_mutex_destroy(&s.death_m);
	cleanup(&s);
	return (exit_code);
}
