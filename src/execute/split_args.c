/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:57:19 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:57:21 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	replace_spaces(char *str, char limiter)
{
	while (*str && *str != limiter)
		str++;
	if (*str)
		str++;
	while (*str && *str != limiter)
	{
		if (*str == ' ')
			*str = -1;
		str++;
	}
	if (*str)
		str++;
	if (*str)
		replace_spaces(str, limiter);
}

static void	restore_space(char **args)
{
	char	*str;

	while (*args)
	{
		str = *args;
		while (*str)
		{
			if (*str == -1)
				*str = ' ';
			if (*str == -2)
				*str = '\'';
			if (*str == -3)
				*str = '"';
			str++;
		}
		args++;
	}
	return ;
}

static void	replace_quote_by_null(char **exec_args)
{
	int	i;

	i = 0;
	while (exec_args[i])
	{
		if (exec_args[i][0] == '"' && exec_args[i][1] == '"')
		{
			exec_args[i][0] = '\'';
			exec_args[i][1] = '\'';
		}
		if (exec_args[i][0] == '\'' && i != 0
			&& exec_args[i][0] == exec_args[i][1] && exec_args[i][2] == '\0')
			exec_args[i][0] = '\0';
		i++;
	}
}

static void	replace_space_and_quote(char *cmd)
{
	replace_spaces(cmd, '\'');
	replace_single_quotes(cmd, '\'');
	replace_spaces(cmd, '"');
	replace_double_quotes(cmd, '"');
}

char	**split_args(char *cmd, t_env **minienv)
{
	char	**exec_args;
	char	*cmd_copy;
	int		i;

	i = 1;
	replace_tabs_with_spaces(cmd);
	if (!contains_quote_mark(cmd))
	{
		cmd_copy = find_and_replace_tilde(cmd, *minienv);
		exec_args = ft_split(cmd_copy, ' ');
		return (free(cmd_copy), exec_args);
	}
	replace_space_and_quote(cmd);
	exec_args = ft_split(cmd, ' ');
	restore_space(exec_args);
	replace_quote_by_null(exec_args);
	if (contains_alpha(exec_args[0]) || ((exec_args[0][0] == '\''
			|| exec_args[0][0] == '"') && exec_args[0][1] == ' '))
		remove_quote(exec_args[0]);
	while (exec_args[i])
	{
		remove_quote(exec_args[i]);
		i++;
	}
	return (exec_args);
}
