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

#include <philo.h>

bool	parse_and_set_int(const char *s, int *n)
{
	int	err;

	if (is_num(s))
	{
		*n = ft_atoi_safe(s, &err);
		return (!err && *n > 0);
	}
	else
		return (false);
}

// accept 4 or 5 arguments
// must be integers greater than zero
bool	parse_args(t_args *a, int argc, const char *argv[])
{
	a->must_eat_num = 0;
	if (argc == 5 || argc == 6)
	{
		return (parse_and_set_int(argv[1], &a->philo_num) && \
			parse_and_set_int(argv[2], &a->time_death) && \
			parse_and_set_int(argv[3], &a->time_eat) && \
			parse_and_set_int(argv[4], &a->time_sleep) && \
			(argc != 6 || parse_and_set_int(argv[5], &a->must_eat_num)));
	}
	else
		return (false);
}

int	main(int argc, const char *argv[])
{
	t_args	args;

	if (!parse_args(&args, argc, argv))
	{
		printf("Bad arguments\n");
		return (0);
	}
	init_and_run(&args);
	return (0);
}
