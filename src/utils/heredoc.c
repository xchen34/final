/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:58:49 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:58:52 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*find_heredoc_pos(char *str)
{
	while (*str && str[1])
	{
		str += skip_quote(str);
		if (*str == '<' && str[1] == '<')
			return (str);
		str++;
	}
	return (NULL);
}

char	*tmp_filename(int heredoc_nbr)
{
	char	file_name[30];
	char	*nbr_str;

	ft_bzero(file_name, 30);
	nbr_str = ft_itoa(heredoc_nbr);
	ft_strlcat(file_name, "/tmp/heredoc", 30);
	ft_strlcat(file_name, nbr_str, 30);
	free(nbr_str);
	return (ft_strdup(file_name));
}

static void	read_heredoc(char *limiter, int heredoc_nbr, int *exit_status,
	t_env *minienv)
{
	char	*line_read;
	char	*file_name;
	int		tmp_file_fd;

	file_name = tmp_filename(heredoc_nbr);
	tmp_file_fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	free(file_name);
	line_read = readline("> ");
	while (line_read && ft_strcmp(line_read, limiter))
	{
		expand_exit_status(&line_read, *exit_status);
		expand_variable(&line_read, minienv);
		ft_putstr_fd(line_read, tmp_file_fd);
		ft_putstr_fd("\n", tmp_file_fd);
		free(line_read);
		line_read = readline("> ");
	}
	if (!line_read)
		print_error_msg("Heredoc limited by EOF, wanted", limiter, ERROR);
	free(line_read);
	close(tmp_file_fd);
	free(limiter);
	free_minienv(&minienv);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}

static int	exec_heredoc(char *limiter, int heredoc_nbr, int *exit_status,
	t_env *minienv)
{
	int	child_pid;

	child_pid = fork();
	define_heredoc_signal(child_pid);
	if (child_pid == -1)
		print_error_msg("fork - heredoc_prompt", limiter, PERROR);
	else if (child_pid == 0)
		read_heredoc(limiter, heredoc_nbr, exit_status, minienv);
	else
	{
		*exit_status = wait_child(child_pid, 1);
		define_main_signal();
		if (*exit_status != 0)
			return (FAILED);
	}
	return (SUCCESS);
}

int	handle_heredoc(char *input, int *exit_status, t_env *minienv)
{
	static int	heredoc_nbr;
	char		*heredoc_pos;
	char		*limiter;

	heredoc_nbr = -1;
	heredoc_pos = find_heredoc_pos(input);
	if (!heredoc_pos)
		return (SUCCESS);
	heredoc_nbr--;
	*heredoc_pos = heredoc_nbr;
	heredoc_pos++;
	limiter = get_label_name(heredoc_pos);
	if (!exec_heredoc(limiter, heredoc_nbr, exit_status, minienv))
	{
		free(limiter);
		return (FAILED);
	}
	free(limiter);
	return (handle_heredoc(input, exit_status, minienv));
}
