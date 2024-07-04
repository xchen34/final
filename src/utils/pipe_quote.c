/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_quote.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:59:06 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:59:09 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	skip_quote(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '\'')
	{
		i++;
		while (str[i] != '\'')
			i++;
	}
	if (str[i] == '"')
	{
		i++;
		while (str[i] != '"')
			i++;
	}
	return (i);
}

static char	*get_next_pipe(char *str)
{
	while (*str)
	{
		str += skip_quote(str);
		if (*str == '|')
			return (str);
		str++;
	}
	return (NULL);
}

int	has_empty_pipe(char *input)
{
	char	*next_pipe;

	next_pipe = get_next_pipe(input);
	if (!next_pipe)
		return (0);
	next_pipe++;
	while (*next_pipe == ' ' || *next_pipe == '\t')
		next_pipe++;
	if (*next_pipe == '|')
	{
		syntax_error("|");
		return (1);
	}
	return (has_empty_pipe(next_pipe));
}

int	has_unclosed_quote(char *str)
{
	char	last_opened;

	last_opened = 0;
	while (*str && !last_opened)
	{
		if (*str == '\'' || *str == '"')
			last_opened = *str;
		str++;
	}
	while (*str && last_opened)
	{
		if (*str && *str == last_opened)
			last_opened = 0;
		str++;
	}
	if (*str)
		return (has_unclosed_quote(str));
	else if (!last_opened)
		return (0);
	else
	{
		ft_putstr_fd("You forgot to close some quotes!\n", STDERR_FILENO);
		return (1);
	}
}
