/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:57:39 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:57:42 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	wait_child(int child_pid, int is_last_child)
{
	int	status;

	if (child_pid == -1)
		return (EXIT_FAILURE);
	if (waitpid(child_pid, &status, 0) == -1)
		print_error_msg("waitpid", ft_itoa(child_pid), PERROR);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			ft_putstr_fd("\n", STDOUT_FILENO);
			return (128 + WTERMSIG(status));
		}
		if (WTERMSIG(status) == SIGQUIT && is_last_child)
		{
			ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
			return (128 + WTERMSIG(status));
		}
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}

int	wait_children(int children_pid[1024])
{
	int	i;
	int	exit_status;
	int	is_last_child;

	i = 0;
	exit_status = 0;
	while (children_pid[i] != 0)
	{
		is_last_child = children_pid[i + 1] == 0;
		exit_status = wait_child(children_pid[i], is_last_child);
		i++;
	}
	close_extra_fds();
	free(children_pid);
	return (exit_status);
}
