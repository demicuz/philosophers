/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psharen <psharen@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:04:24 by psharen           #+#    #+#             */
/*   Updated: 2022/06/24 22:28:17 by psharen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

// TODO remove unused headers
#include <stdio.h>
#include <stdlib.h>

bool	parse_and_set_int(const char *s, int *n)
{
	int	err;

	err = 0;
	if (is_num(s))
	{
		*n = ft_atoi_safe(s, &err);
		return (!err && *n >= 0);
	}
	else
		return (false);
}

bool	parse_and_set_uint(const char *s, unsigned int *n)
{
	int	err;
	int	r;

	err = 0;
	if (is_num(s))
	{
		r = ft_atoi_safe(s, &err);
		*n = r;
		return (!err && r >= 0);
	}
	else
		return (false);
}

// accept 4 or 5 arguments
// must be integers equal to or greater than zero
// the code is ugly af
bool	parse_args(t_args *a, int argc, const char *argv[])
{
	a->must_eat_num = -1;
	if (argc == 5)
	{
		return (parse_and_set_uint(argv[1], &a->philo_num) && \
			parse_and_set_uint(argv[2], &a->time_death) && \
			parse_and_set_uint(argv[3], &a->time_eat) && \
			parse_and_set_uint(argv[4], &a->time_sleep));
	}
	else if (argc == 6)
	{
		return (parse_and_set_uint(argv[1], &a->philo_num) && \
			parse_and_set_uint(argv[2], &a->time_death) && \
			parse_and_set_uint(argv[3], &a->time_eat) && \
			parse_and_set_uint(argv[4], &a->time_sleep) && \
			parse_and_set_int(argv[5], &a->must_eat_num));
	}
	else
		return (false);
}

int	main(int argc, const char *argv[])
{
	t_args	args;
	t_state	state;

	if (!parse_args(&args, argc, argv) || args.philo_num > 254)
	{
		printf(HELP_MESSAGE);
		return (EXIT_FAILURE);
	}
	if (args.must_eat_num == 0 || args.philo_num == 0)
		return (EXIT_SUCCESS);
	if (!init(&args, &state))
	{
		printf("Failed to initialize\n");
		return (EXIT_FAILURE);
	}
	if (!run_simulation(&args, &state))
	{
		printf("Failed to run the simulation\n");
		return (EXIT_FAILURE);
	}
	cleanup(&args, &state);
	return (EXIT_SUCCESS);
}
