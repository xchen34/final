/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:57:33 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:57:35 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static size_t	calculate_cmd_len(char *cmd, t_env *minienv)
{
	char	*tilde_pos;
	size_t	new_cmd_len;
	size_t	home_len;

	tilde_pos = cmd;
	new_cmd_len = 0;
	home_len = ft_strlen(minienv_value("__HOME", minienv));
	while (*tilde_pos)
	{
		if (*tilde_pos == '~')
			new_cmd_len += home_len;
		else
			new_cmd_len++;
		tilde_pos++;
	}
	return (new_cmd_len);
}

static void	expand_tilde_path(char *new_cmd_ptr, char *cmd_ptr,
	char *expanded_path, t_env *minienv)
{
	size_t	home_len;
	int		escape_flag;

	home_len = ft_strlen(minienv_value("__HOME", minienv));
	escape_flag = 0;
	while (*cmd_ptr)
	{
		if (*cmd_ptr == '\\' && !escape_flag)
		{
			escape_flag = 1;
			cmd_ptr++;
		}
		else if (*cmd_ptr == '~' && !escape_flag && expanded_path != NULL)
		{
			ft_memcpy(new_cmd_ptr, expanded_path, home_len);
			new_cmd_ptr += home_len;
			cmd_ptr++;
		}
		else
		{
			*new_cmd_ptr++ = *cmd_ptr++;
			escape_flag = 0;
		}
	}
	*new_cmd_ptr = '\0';
}

char	*find_and_replace_tilde(char *cmd, t_env *minienv)
{
	char	*cmd_ptr;
	char	*new_cmd;
	char	*new_cmd_ptr;
	char	*expanded_path;
	size_t	new_cmd_len;

	new_cmd_len = calculate_cmd_len(cmd, minienv);
	new_cmd = malloc(new_cmd_len + 1);
	if (!new_cmd)
		return (cmd);
	new_cmd_ptr = new_cmd;
	cmd_ptr = cmd;
	expanded_path = minienv_value("__HOME", minienv);
	expand_tilde_path(new_cmd_ptr, cmd_ptr, expanded_path, minienv);
	return (new_cmd);
}
