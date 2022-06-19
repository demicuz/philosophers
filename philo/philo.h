/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:02:24 by psharen           #+#    #+#             */
/*   Updated: 2022/06/20 00:04:14 by psharen          ###   ########.fr       */
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

# define HELP_MESSAGE "\
Bad arguments\n\
Usage: ./philo philo_num t_death t_eat t_sleep [must_eat_num]\n\
Examples: ./philo 5 800 200 200\n\
          ./philo 5 800 200 200 1\n"

typedef struct s_args
{
	int	philo_num;
	int	time_death;
	int	time_eat;
	int	time_sleep;
	int	must_eat_num;
}	t_args;

typedef struct s_philo
{
	int				index;
	t_args			*args;
	struct timeval	*start;
	int				times_eaten;

	pthread_mutex_t	*fork1_for_me;
	pthread_mutex_t	*fork2_for_me;
	pthread_mutex_t	*fork1_for_neighbor;
	pthread_mutex_t	*fork2_for_neighbor;
	pthread_mutex_t	*all_forks;

	long			last_eaten;
	pthread_mutex_t	*last_eaten_m;

	bool			*death;
	pthread_mutex_t	*death_m;

	int				*philos_still_eating;
	pthread_mutex_t	*still_eating_m;
}	t_philo;

typedef struct s_state
{
	t_args			*args;
	struct timeval	start;

	pthread_t		*philos;
	pthread_t		*death_checkers;
	t_philo			*philos_data;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*last_eaten;

	pthread_mutex_t	death_m;
	bool			death;

	pthread_mutex_t	still_eating_m;
	int				philos_still_eating;
}	t_state;

typedef void	*(*t_philo_routine)(void *philo_data);

// utils
int		is_num(const char *s);
int		ft_atoi_safe(const char *str, int *err);
long	time_passed(struct timeval *start);
void	unlock_all_mutexes(pthread_mutex_t *mutexes, int n);

// bootstrap
bool	state_malloc(t_state *s, int philo_num);
void	init_philo_data(t_state *s, t_args *args);
void	destroy_mutexes(pthread_mutex_t *mutexes, int n);
bool	init_vars(t_state *s, t_args *args);
void	give_forks(t_philo *philos_data, pthread_mutex_t *forks, int philo_num);
bool	init_eat_death_mutexes(t_state *s, t_args *args);
bool	init_mutexes(t_state *s, t_args *args);

// philo_actions
void	take_forks(t_philo *p);
void	eat(t_philo *p);
// void	release_forks(t_philo *p);
void	take_a_nap(t_philo *p);

// routines
void	lock_and_check_death(t_philo *p);
void	*routine(void *philo_data);
void	terminate_if_all_eaten(t_philo *p);
void	*routine_min_eaten(void *philo_data);
void	die(t_philo *p, long now_millis);
void	*routine_death(void *philo_data);

// run
bool	run_simulation(t_state *s, int philo_num);
void	join_all(pthread_t *philos, pthread_t *death_checkers, int philo_num);
void	cleanup(t_state *s);
int		init_and_run(t_args *args);

#endif
