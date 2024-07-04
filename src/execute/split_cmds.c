/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:57:25 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:57:27 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	replace_pipe(char *str, char limiter)
{
	while (*str && *str != limiter)
		str++;
	if (*str)
		str++;
	while (*str && *str != limiter)
	{
		if (*str == '|')
			*str = -1;
		str++;
	}
	if (*str)
		str++;
	if (*str)
		replace_pipe(str, limiter);
}

static void	restore_pipe(char **args)
{
	char	*str;

	while (*args)
	{
		str = *args;
		while (*str)
		{
			if (*str == -1)
				*str = '|';
			str++;
		}
		args++;
	}
	return ;
}

char	**split_cmds(char *input)
{
	char	**cmds;

	replace_pipe(input, '"');
	replace_pipe(input, '\'');
	cmds = ft_split(input, '|');
	restore_pipe(cmds);
	return (cmds);
}
