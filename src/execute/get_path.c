/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:56:56 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:56:59 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_path(char *cmd)
{
	if (*cmd == '.')
		cmd++;
	if (*cmd == '.')
		cmd++;
	if (*cmd == '/')
		return (1);
	return (0);
}

static void	create_path(char *base, char *part1, char *part2, char *part3)
{
	*base = '\0';
	ft_strlcat(base, part1, PATH_MAX);
	ft_strlcat(base, part2, PATH_MAX);
	ft_strlcat(base, part3, PATH_MAX);
}

static char	*local_path(char *cmd, t_env *minienv)
{
	char	full_path[PATH_MAX];

	if (*cmd == '/')
		return (ft_strdup(cmd));
	full_path[0] = '\0';
	create_path(full_path, minienv_value("PWD", minienv), "/", cmd);
	return (ft_strdup(cmd));
}

char	*get_path(char *cmd, t_env *minienv)
{
	char	*path_env;
	char	**path;
	char	current_path[PATH_MAX];
	char	**path_start;

	if (is_path(cmd))
		return (local_path(cmd, minienv));
	path_env = minienv_value("PATH", minienv);
	if (!path_env)
	{
		if (ft_strcmp(getcwd(current_path, sizeof(current_path)), "/usr/bin"))
			exit_error(cmd, "No such file or directory", 127, ERROR);
		else
			path_env = getcwd(current_path, sizeof(current_path));
	}	
	path = ft_split(path_env, ':');
	path_start = path;
	while (*path)
	{
		create_path(current_path, *path, "/", cmd);
		if (access(current_path, F_OK) == 0)
			return (free_array(path_start), ft_strdup(current_path));
		path++;
	}
	return (free_array(path_start), NULL);
}
