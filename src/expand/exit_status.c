/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:57:48 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:57:50 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*find_exit_status_position(char *input)
{
	while (*input)
	{
		if (*input == '\'')
		{
			input++;
			while (*input && *input != '\'')
				input++;
		}
		if (*input == '\"')
		{
			input++;
			while (*input && *input != '\"')
			{
				if (*input == '$' && input[1] == '?')
					return (input);
				input++;
			}
		}
		if (*input == '$' && input[1] == '?')
			return (input);
		input++;
	}
	return (NULL);
}

static void	update_input(char **input, char *exit_code, char *second_part)
{
	char	*first_part;
	char	*updated_input;

	first_part = ft_strjoin(*input, exit_code);
	updated_input = ft_strjoin(first_part, second_part);
	free(*input);
	free(first_part);
	*input = updated_input;
}

void	expand_exit_status(char **input, int exit_status)
{
	char	*exit_status_pos;
	char	*exit_status_str;

	exit_status_pos = find_exit_status_position(*input);
	if (exit_status_pos)
	{
		*exit_status_pos = '\0';
		exit_status_str = ft_itoa(exit_status);
		update_input(input, exit_status_str, (exit_status_pos + 2));
		free(exit_status_str);
		expand_exit_status(input, exit_status);
	}
}
