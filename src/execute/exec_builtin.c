/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:56:24 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:56:26 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", ft_strlen(cmd) + 1) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", ft_strlen(cmd) + 1) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", ft_strlen(cmd) + 1) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", ft_strlen(cmd) + 1) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", ft_strlen(cmd) + 1) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", ft_strlen(cmd) + 1) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", ft_strlen(cmd) + 1) == 0)
		return (1);
	return (0);
}

int	exec_builtin(char **args, t_env **minienv)
{
	char	*cmd;

	cmd = args[0];
	if (ft_strncmp(cmd, "echo", ft_strlen(cmd) + 1) == 0)
		return (echo(args));
	if (ft_strncmp(cmd, "cd", ft_strlen(cmd) + 1) == 0)
		return (cd(args, *minienv));
	if (ft_strncmp(cmd, "pwd", ft_strlen(cmd) + 1) == 0)
		return (pwd(*minienv));
	if (ft_strncmp(cmd, "export", ft_strlen(cmd) + 1) == 0)
		return (export(args, minienv));
	if (ft_strncmp(cmd, "unset", ft_strlen(cmd) + 1) == 0)
		return (unset(args, minienv));
	if (ft_strncmp(cmd, "env", ft_strlen(cmd)+ 1) == 0)
		return (env(args, *minienv));
	if (ft_strncmp(cmd, "exit", ft_strlen(cmd) + 1) == 0)
		return (builtin_exit(args, minienv));
	else
		return (EXIT_FAILURE);
}
