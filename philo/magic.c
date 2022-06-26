/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 03:17:38 by psharen           #+#    #+#             */
/*   Updated: 2022/06/26 04:00:27 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

// *magic*
// Using stupid hacks, yes.
void	magic_start_delay(t_args *a, unsigned int index)
{
	if (a->philo_num % 2 == 0 && index % 2 == 1)
		usleep(MAGIC_START_DELAY);
	else if (a->philo_num % 2 != 0 && index == a->philo_num - 1)
		usleep(a->time_eat * 1000 + PHILO_WAIT_TIME);
	else if (a->philo_num % 2 != 0 && index % 2 == 1)
		usleep(MAGIC_START_DELAY);
}

// Top naming in action. This is used only for uneven number of philos.
// Before searching for forks, each philo must wait exactly this time:
// 1000 * max(2 * a->time_eat, a->time_sleep) + PHILO_WAIT_TIME
// But he should also print that he's thinking! So I can't just `usleep()`.
unsigned int	get_magic_wait_time(t_args *a)
{
	int	wat;

	wat = 2 * a->time_eat - a->time_sleep;
	if (a->philo_num % 2 != 0 && wat >= 0)
		return (wat * 1000 + PHILO_WAIT_TIME);
	else
		return (0);
}
