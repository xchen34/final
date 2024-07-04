/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:55:56 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:55:58 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	fit_in_long_long(char *str)
{
	long long	result;
	int			nbr;

	if (ft_strlen(str) > 20)
		return (0);
	if (ft_strcmp(str, "-9223372036854775808") == 0)
		return (1);
	result = 0;
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		nbr = *str - '0';
		if (result > (LLONG_MAX - nbr) / 10)
			return (0);
		result = result * 10 + nbr;
		str++;
	}
	return (1);
}

static int	check_args_error(char **args, t_env **minienv)
{
	if (!args || !args[1])
	{
		if (args)
			free_array(args);
		close_all_fds();
		free_minienv(minienv);
		exit(EXIT_SUCCESS);
	}
	if (args[1] && args[2])
	{
		if (!fit_in_long_long(args[1]))
		{
			free_array(args);
			free_minienv(minienv);
			exit_error("exit", "numeric argument required", 2, ERROR);
		}
		return (print_error_msg("exit", "too many arguments", ERROR), 1);
	}
	if (!fit_in_long_long(args[1]) || args[1][0] == '\0')
	{
		free_array(args);
		free_minienv(minienv);
		exit_error("exit", "numeric argument required", 2, ERROR);
	}
	return (0);
}

int	builtin_exit(char **args, t_env **minienv)
{
	int	exit_status;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (check_args_error(args, minienv) == 1)
		return (EXIT_FAILURE);
	rl_clear_history();
	free_minienv(minienv);
	close_all_fds();
	exit_status = ft_atoll(args[1]);
	free_array(args);
	exit(exit_status);
}
