/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 19:02:20 by psharen           #+#    #+#             */
/*   Updated: 2022/06/24 22:35:36 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <sys/time.h>
# include <sys/types.h>
# include <semaphore.h>
# include <stdbool.h>

# define HELP_MESSAGE "\
Bad arguments!\n\
\n\
Usage: ./philo_bonus philo_num t_death t_eat t_sleep [must_eat_num]\n\
arguments are unsigned ints, must_eat_num is a signed int.\n\
philo_num shouldn't be greater than 254.\n\
\n\
Examples: ./philo_bonus 5 800 200 200\n\
          ./philo_bonus 5 800 200 200 1\n"

// TODO maybe other than 100? And rename this
# define CHECKER_WAIT_TIME 100
# define PHILO_WAIT_TIME 500

typedef struct s_args
{
	unsigned int	philo_num;
	unsigned int	time_death;
	unsigned int	time_eat;
	unsigned int	time_sleep;
	int				must_eat_num;
}	t_args;

typedef struct s_state
{
	pid_t	*pids;
	// struct timeval	start;
	// mutexes
	struct timeval	*start;
	sem_t	*forks;
	sem_t	*stdout;

	sem_t	**last_eaten_sems;
	char	**last_eaten_sem_names;
}	t_state;

// Some stuff duplicates t_state, but unfortunately that's needed for
// routine_death() thread to work
typedef struct s_philo
{
	// TODO remove unused variables
	t_args			*args;
	struct timeval	*start;

	unsigned int	index;
	int				times_eaten;

	sem_t			*last_eaten_sem;
	unsigned long	last_eaten;
	// bool			died;

	// TODO duplicating t_state
	sem_t	*stdout;
}	t_philo;

// bootstrap
bool	init(t_args *a, t_state *s);

// run
bool	wait_simulation_end(t_args *a, t_state *s);
bool	run_simulation(t_args *a, t_state *s);

// philo
void	*routine_death(void *philo_data);
void	set_philo_vars(t_philo *p, t_args *a, t_state *s, unsigned int index);
void	init_philo(t_philo *p, t_args *a, t_state *s, unsigned int index);
void	philo_routine(t_args *a, t_state *s, unsigned int index);

// philo_actions
void	think(t_philo *p, t_state *s);
void	take_forks(t_philo *p, t_state *s);
void	eat(t_philo *p, t_args *a, t_state *s);
void	take_a_nap(t_philo *p, t_args *a, t_state *s);

// utils
int		is_num(const char *s);
int		ft_atoi_safe(const char *str, int *err);
int		ft_count_digits(int n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
unsigned long	time_passed(struct timeval *start);
void	kill_all(pid_t *pids, int n);
char	*get_philo_sem_name(int index);
void	magic_start_delay(t_args *a, unsigned int index);
unsigned int	get_magic_wait_time(t_args *a);
void	cleanup(t_args *a, t_state *s);

#endif
