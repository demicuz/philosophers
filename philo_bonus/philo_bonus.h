/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 19:02:20 by psharen           #+#    #+#             */
/*   Updated: 2022/06/24 02:34:01 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

#include <sys/time.h>
#include <sys/types.h>
#include <semaphore.h>
#include <stdbool.h>

# define HELP_MESSAGE "\
Bad arguments\n\
Usage: ./philo_bonus philo_num t_death t_eat t_sleep [must_eat_num]\n\
Examples: ./philo_bonus 5 800 200 200\n\
          ./philo_bonus 5 800 200 200 1\n"

// TODO maybe other than 100? And rename this
#define MIN_WAIT_TIME 100
#define PHILO_WAIT_TIME 100

#define THREAD_CREATION_FAILED 255

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

	int				index;
	int				times_eaten;

	sem_t			*last_eaten_sem;
	unsigned long	last_eaten;
	// bool			died;

	// TODO duplicating t_state
	sem_t	*stdout;
}	t_philo;

// bootstrap
bool	init(t_args *a, t_state *s);

// utils
int		is_num(const char *s);
int		ft_atoi_safe(const char *str, int *err);
// unsigned long	time_passed(struct timeval *start);
unsigned long	time_passed(struct timeval *start);
void	kill_all(pid_t *pids, int n);
int		ft_count_digits(int n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	*get_philo_sem_name(int index);

// void	free_names(char **last_eaten_sem_names, int n);
void	cleanup(t_args *a, t_state *s);

// philo_actions
void	lock_and_check_death(t_philo *p, t_state *s);
void	take_forks(t_philo *p, t_state *s);
void	eat(t_philo *p, t_args *a, t_state *s);
void	take_a_nap(t_philo *p, t_args *a, t_state *s);

#endif
