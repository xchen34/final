/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:56:35 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:56:37 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	external_exit(char **args, t_env *minienv, int exit_status)
{
	if (exit_status == 126)
		print_error_msg(args[0], "Is a directory", ERROR);
	if (exit_status == 127)
		print_error_msg(args[0], "command not found", ERROR);
	rl_clear_history();
	free_minienv(&minienv);
	free_array(args);
	close_all_fds();
	exit(exit_status);
}

static int	is_folder(char *cmd)
{
	struct stat	statbuf;

	if (stat(cmd, &statbuf) != 0)
		return (0);
	if (S_ISDIR(statbuf.st_mode))
	{
		if (*cmd == '.')
			cmd++;
		if (*cmd == '.')
			cmd++;
		if (*cmd == '/')
			return (1);
	}
	return (0);
}

static void	handle_execve_errors(char **args, char *path, char **env)
{
	int	exit_status;

	exit_status = EXIT_FAILURE;
	print_error_msg("execve", args[0], PERROR);
	if (access(path, F_OK) != 0)
		exit_status = 127;
	else if (access(path, X_OK) != 0)
		exit_status = 126;
	rl_clear_history();
	free_array(args);
	free_array(env);
	free(path);
	exit(exit_status);
}

int	exec_external(char **args, t_env *minienv)
{
	char	*path;
	char	*cmd;
	char	**env;

	cmd = args[0];
	if (!cmd)
		external_exit(args, minienv, 0);
	if (is_empty(args[0]))
		external_exit(&args[0], minienv, 127);
	if (is_folder(cmd))
		external_exit(args, minienv, 126);
	path = get_path(cmd, minienv);
	if (path == NULL && minienv_value("PATH", minienv))
		external_exit(args, minienv, 127);
	else if (path == NULL)
		path = ft_strdup(cmd);
	rl_clear_history();
	close_extra_fds();
	env = minienv_to_env(minienv);
	free_minienv(&minienv);
	if (execve(path, args, env) == -1)
		handle_execve_errors(args, path, env);
	exit(EXIT_SUCCESS);
}
