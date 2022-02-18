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
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
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
	pthread_mutex_t *still_eating_m;

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
	pthread_mutex_t still_eating_m;
}	t_state;

typedef void	*(*philo_routine)(void *philo_data);

// utils
int		is_num(const char *s);
int		ft_atoi_safe(const char *str, int *err);
long	time_passed(struct timeval *start);

// bootstrap
bool	state_malloc(t_state *s, int philo_num);
void	init_philo_data(t_state *s, t_args *args);
void	destroy_mutexes(pthread_mutex_t *forks, int n);
bool	init_vars(t_state *s, t_args *args);
void	give_forks(t_philo *philos_data, pthread_mutex_t *forks, int philo_num);

// philo_actions
void	take_forks(t_philo *p);
void	eat(t_philo *p);
void	release_forks(t_philo *p);
void	take_a_nap(t_philo *p);

// routines
void	*routine(void *philo_data);
void	unlock_all_forks(pthread_mutex_t *forks, int n);
void	terminate_if_all_eaten(t_philo *p);
void	*routine_min_eaten(void *philo_data);
void	*routine_death(void *philo_data);

// run
bool	run_simulation(t_state *s, int philo_num);
void	join_all(pthread_t *philos, pthread_t *death_checkers, int philo_num);
void	free_state(t_state *s);
void	init_and_run(t_args *args);


#endif
