/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:56:49 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:56:51 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	handle_put_redirect(char *cmd, int fds[2], int put)
{
	if (put == 0)
	{
		if (fds[0] == -1)
			fds[0] = dup(STDIN_FILENO);
		if (!redirect_input(cmd))
		{
			redirect_fd(fds[0], STDIN_FILENO);
			return (FAILED);
		}
	}
	else if (put == 1)
	{
		if (fds[1] == -1)
			fds[1] = dup(STDOUT_FILENO);
		if (!redirect_output(cmd))
		{
			redirect_fd(fds[1], STDOUT_FILENO);
			return (FAILED);
		}
	}
	return (SUCCESS);
}

static int	handle_redirects(char *cmd, int fds[2])
{
	char	redirect;

	fds[0] = -1;
	fds[1] = -1;
	redirect = get_next_redirect(cmd);
	while (redirect)
	{
		if (redirect == '<' && !handle_put_redirect(cmd, fds, 0))
			return (FAILED);
		if (redirect == '>' && !handle_put_redirect(cmd, fds, 1))
			return (FAILED);
		if (redirect < 0)
		{
			if (fds[0] == -1)
				fds[0] = dup(STDIN_FILENO);
			redirect_heredoc(cmd, redirect);
		}
		redirect = get_next_redirect(cmd);
	}
	return (SUCCESS);
}

static void	restore_fds(int fds[2])
{
	if (fds[0] != -1)
		redirect_fd(fds[0], STDIN_FILENO);
	if (fds[1] != -1)
		redirect_fd(fds[1], STDOUT_FILENO);
}

static int	exec_regular_cmd(char **args, t_env *minienv)
{
	int		child_pid;
	char	*cmd;

	cmd = args[0];
	child_pid = fork();
	define_exec_signal(child_pid);
	if (child_pid == -1)
		print_error_msg("fork", cmd, PERROR);
	else if (child_pid == 0)
		exec_external(args, minienv);
	else
		return (wait_child(child_pid, 1));
	exit(EXIT_FAILURE);
}

int	exec_one_cmd(char *cmd, t_env **minienv)
{
	char	**args;
	int		exit_status;
	int		fds[2];

	if (!handle_redirects(cmd, &fds[0]))
	{
		restore_fds(fds);
		free(cmd);
		return (EXIT_FAILURE);
	}
	args = split_args(cmd, minienv);
	free(cmd);
	if (is_builtin(args[0]))
		exit_status = exec_builtin(args, minienv);
	else
		exit_status = exec_regular_cmd(args, *minienv);
	free_array(args);
	restore_fds(fds);
	return (exit_status);
}
