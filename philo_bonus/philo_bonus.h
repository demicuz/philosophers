/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 19:02:20 by psharen           #+#    #+#             */
/*   Updated: 2022/06/26 08:23:57 by psharen          ###   ########.fr       */
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

// In microseconds
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
	pid_t			*pids;
	struct timeval	start;

	sem_t			*forks;
	sem_t			*stdout;
	sem_t			**last_eaten_sems;
	char			**last_eaten_sem_names;
}	t_state;

// Some stuff duplicates t_state, but unfortunately that's needed for
// routine_death() thread to work
typedef struct s_philo
{
	t_args			*args;
	struct timeval	*start;
	unsigned int	index;

	unsigned long	last_eaten;
	sem_t			*last_eaten_sem;
	sem_t			*stdout;

	int				times_eaten;
}	t_philo;

// Three tabs. Yes. Norminette. Stage: bargain.

// bootstrap
bool			open_philo_semaphores(t_args *a, t_state *s);
bool			init_philo_semaphores(t_args *a, t_state *s);
bool			init(t_args *a, t_state *s);

// run
bool			wait_simulation_end(t_args *a, t_state *s);
bool			run_simulation(t_args *a, t_state *s);

// philo
void			die(t_philo *p);
void			*routine_death(void *philo_data);
void			set_philo_vars(t_philo *p, t_args *a, t_state *s,
					unsigned int index);
void			init_philo(t_philo *p, t_args *a, t_state *s,
					unsigned int index);
void			philo_routine(t_args *a, t_state *s, unsigned int index);

// philo_actions
void			think(t_philo *p, t_state *s);
void			take_forks(t_philo *p, t_state *s);
void			eat(t_philo *p, t_args *a, t_state *s);
void			take_a_nap(t_philo *p, t_args *a, t_state *s);

// utils
int				is_num(const char *s);
int				ft_atoi_safe(const char *str, int *err);
int				ft_count_digits(int n);
void			*ft_memcpy(void *dest, const void *src, size_t n);
void			kill_all(pid_t *pids, int n);
char			*get_philo_sem_name(int index);
void			cleanup(t_args *a, t_state *s);

unsigned long	time_passed(struct timeval *start);
unsigned int	get_magic_wait_time(t_args *a);

#endif
