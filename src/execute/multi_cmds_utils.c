/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_cmds_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:57:03 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:57:06 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	arr_len(char **arr)
{
	int	len;

	len = 0;
	while (*arr)
	{
		len++;
		arr++;
	}
	return (len);
}

int	*init_children_pid(char **cmds)
{
	int		*children_pid;
	size_t	size;

	size = sizeof(int) * (arr_len(cmds) + 1);
	children_pid = malloc(size);
	if (!children_pid)
		return (NULL);
	ft_bzero(children_pid, size);
	return (children_pid);
}

void	handle_pipe(int fd_out, char *current_cmd, char **cmds)
{
	int			is_first_cmd;
	int			has_next_cmd;
	char		*last_cmd;
	static int	pipe_fds[2];

	last_cmd = cmds[arr_len(cmds) - 1];
	is_first_cmd = (current_cmd == cmds[0]);
	has_next_cmd = (current_cmd != last_cmd);
	if (!is_first_cmd)
		redirect_fd(pipe_fds[0], STDIN_FILENO);
	if (has_next_cmd)
	{
		if (pipe(pipe_fds) == -1)
			print_error_msg("pipe", current_cmd, PERROR);
		redirect_fd(pipe_fds[1], STDOUT_FILENO);
	}
	else
		redirect_fd(fd_out, STDOUT_FILENO);
}
