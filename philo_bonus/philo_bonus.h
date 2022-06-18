/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 19:02:20 by psharen           #+#    #+#             */
/*   Updated: 2022/06/17 16:48:02 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

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

// utils
int	is_num(const char *s);
int	ft_atoi_safe(const char *str, int *err);

#endif
