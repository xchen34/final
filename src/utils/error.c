/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:58:33 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:58:35 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_error_msg(char *cmd, char *msg, int print_error)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (print_error == ERROR)
	{
		ft_putstr_fd(msg, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	if (print_error == PERROR)
		perror(msg);
}

void	exit_error(char *cmd, char *msg, int error, int print_error)
{
	if (print_error == ERROR)
		print_error_msg(cmd, msg, ERROR);
	if (print_error == PERROR)
		print_error_msg(cmd, msg, PERROR);
	close_all_fds();
	rl_clear_history();
	exit(error);
}

int	has_input_error(char *input, int *exit_status, t_env *minienv)
{
	int	result;

	result = 0;
	if (is_empty(input))
		result = 1;
	else if (has_unclosed_quote(input))
		result = 1;
	else if (is_invalid_syntax(input))
	{
		*exit_status = 2;
		result = 1;
	}
	else if (!handle_heredoc(input, exit_status, minienv))
		result = 1;
	if (result == 1)
		free(input);
	return (result);
}

int	exit_varname_error_msg(char *cmd, char *varname)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (ft_strncmp(cmd, "export", ft_strlen(cmd)) == 0)
	{
		ft_putstr_fd(": \'", STDERR_FILENO);
		ft_putstr_fd(varname, STDERR_FILENO);
		ft_putstr_fd("'", STDERR_FILENO);
		ft_putstr_fd(": not a valid identifier", STDERR_FILENO);
	}
	if (ft_strncmp(cmd, "unset", ft_strlen(cmd)) == 0)
	{
		ft_putstr_fd(varname, STDERR_FILENO);
		ft_putstr_fd(": invalid parameter name", STDERR_FILENO);
	}
	ft_putstr_fd("\n", STDERR_FILENO);
	return (EXIT_FAILURE);
}
