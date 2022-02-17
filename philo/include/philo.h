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

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>

typedef struct s_args
{
	int	philo_num;
	int	time_death;
	int	time_eat;
	int	time_sleep;
	int	must_eat_num;
}	t_args;

typedef	struct s_philo
{
	pthread_mutex_t	*fork1_for_me;
	pthread_mutex_t	*fork2_for_me;
	pthread_mutex_t	*fork1_for_neighbor;
	pthread_mutex_t	*fork2_for_neighbor;
	pthread_mutex_t	*all_forks;
	struct timeval	*start;
	long			last_eaten;
	int				times_eaten;
	int				index;

	bool			*death;
	int				*philos_still_eating;
	t_args			*args;
	// pthread_mutex_t *still_eating_m;

}	t_philo;

typedef struct s_state
{
	pthread_t		*philos;
	pthread_t		*death_checkers;
	t_philo			*philos_data;
	pthread_mutex_t	*forks;
	struct timeval	start;
	bool			death;
	t_args			*args;
	int				philos_still_eating;
	// pthread_mutex_t still_eating_m;
}	t_state;

typedef void	*(*philo_routine)(void *philo_data);

#endif
