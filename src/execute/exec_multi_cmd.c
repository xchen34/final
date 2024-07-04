/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multi_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:56:41 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:56:43 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	handle_redirects(char *cmd, char **cmds, t_env **minienv)
{
	char	redirect;

	redirect = get_next_redirect(cmd);
	while (redirect)
	{
		if ((redirect == '<' && !redirect_input(cmd))
			|| (redirect == '>' && !redirect_output(cmd)))
		{
			rl_clear_history();
			free_minienv(minienv);
			free_array(cmds);
			close_all_fds();
			exit(EXIT_FAILURE);
		}
		if (redirect < 0)
			redirect_heredoc(cmd, redirect);
		redirect = get_next_redirect(cmd);
	}
}

static void	exec_regular_cmds(char *cmd, char **cmds,
		t_env **minienv)
{
	char	**args;
	int		exit_status;

	close_extra_fds();
	args = split_args(cmd, minienv);
	free_array(cmds);
	if (is_builtin(args[0]))
	{
		exit_status = exec_builtin(args, minienv);
		free_array(args);
		free_minienv(minienv);
		rl_clear_history();
		exit(exit_status);
	}
	else
		exec_external(args, *minienv);
}

static void	restore_fds(int fds[2])
{
	redirect_fd(fds[0], STDIN_FILENO);
	redirect_fd(fds[1], STDOUT_FILENO);
}

int	exec_multi_cmds(char **cmds, t_env **minienv)
{
	int	fds[2];
	int	*children_pid;
	int	i;

	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
	children_pid = init_children_pid(cmds);
	i = 0;
	while (cmds[i])
	{
		handle_pipe(fds[1], cmds[i], cmds);
		children_pid[i] = fork();
		define_exec_signal(children_pid[i]);
		if (children_pid[i] == -1)
			print_error_msg("fork", cmds[i], PERROR);
		else if (children_pid[i] == 0)
		{
			free(children_pid);
			handle_redirects(cmds[i], cmds, minienv);
			exec_regular_cmds(cmds[i], cmds, minienv);
		}
		i++;
	}
	restore_fds(fds);
	return (wait_children(children_pid));
}
