/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:58:40 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:58:42 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	redirect_fd(int fd_redirect, int fd_location)
{
	dup2(fd_redirect, fd_location);
	close(fd_redirect);
}

void	close_extra_fds(void)
{
	int	last_open_fd;

	last_open_fd = open("/tmp/lastfd", O_RDWR | O_CREAT, 0777);
	if (last_open_fd == -1)
		print_error_msg("open", "/tmp/lastfd", PERROR);
	while (last_open_fd > STDERR_FILENO)
	{
		close(last_open_fd);
		last_open_fd--;
	}
}

void	close_all_fds(void)
{
	close_extra_fds();
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}
