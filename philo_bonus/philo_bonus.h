/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 19:02:20 by psharen           #+#    #+#             */
/*   Updated: 2022/06/20 01:22:40 by psharen          ###   ########.fr       */
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

typedef struct s_args
{
	int	philo_num;
	int	time_death;
	int	time_eat;
	int	time_sleep;
	int	must_eat_num;
}	t_args;

typedef struct s_state
{
	pid_t	*pids;
	// struct timeval	start;
	// mutexes
	sem_t	*forks;
	sem_t	*stdout;

	sem_t	**last_eaten_sems;
	char	**last_eaten_sem_names;
}	t_state;

typedef struct s_philo
{
	int		index;
	int		times_eaten;

	long	last_eaten;
	sem_t	*last_eaten_sem;
}	t_philo;

// bootstrap
bool	init(t_args *a, t_state *s);

// utils
int		is_num(const char *s);
int		ft_atoi_safe(const char *str, int *err);
long	time_passed(struct timeval *start);
void	kill_all(pid_t *pids, int n);
int		ft_count_digits(int n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	*get_philo_sem_name(int index);

// void	free_names(char **last_eaten_sem_names, int n);
void	cleanup(t_args *a, t_state *s);

#endif
