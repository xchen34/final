/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:55:33 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:55:35 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	get_directory_path(char *path_name, t_env *minienv)
{
	char	*pwd;
	char	*oldpwd;

	pwd = minienv_value("PWD", minienv);
	oldpwd = minienv_value("OLDPWD", minienv);
	if (!pwd)
		return ;
	if (oldpwd && *pwd)
		minienv_update("OLDPWD", pwd, minienv);
	if (*pwd)
		minienv_update("PWD", getcwd(path_name, PATH_MAX), minienv);
}

char	*get_cd_path(char **args, t_env *minienv)
{
	char	*path;

	path = NULL;
	if (args[1] && args[2])
	{
		print_error_msg("cd", "too many arguments", ERROR);
		return (NULL);
	}
	if (args[1])
	{
		path = args[1];
		if (ft_strcmp(path, "") == 0)
			path = minienv_value("PWD", minienv);
	}
	else if (args[0])
		path = minienv_value("__HOME", minienv);
	if (!path)
	{
		print_error_msg("cd", "HOME not set", ERROR);
		return (NULL);
	}
	if (ft_strcmp(path, "--") == 0)
		path = minienv_value("__HOME", minienv);
	return (path);
}

int	cd(char **args, t_env *minienv)
{
	char	*path;
	char	path_name[PATH_MAX];

	path = get_cd_path(args, minienv);
	if (!path)
		return (1);
	if (ft_strcmp(path, "-") == 0)
	{
		path = minienv_value("OLDPWD", minienv);
		if (path)
		{
			ft_putstr_fd(path, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		else
			return (print_error_msg("cd", "OLDPWD not set", ERROR), 1);
	}
	if (chdir(path) != 0)
	{
		remove_quote(path);
		return (print_error_msg("cd", path, PERROR), EXIT_FAILURE);
	}
	get_directory_path(path_name, minienv);
	return (0);
}
