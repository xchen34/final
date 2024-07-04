/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:59:25 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:59:27 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	contains_alpha(char *args)
{
	while (*args)
	{
		if (ft_isalpha(*args))
			return (1);
		args++;
	}
	return (0);
}

int	contains_quote_mark(char *str)
{
	if (!str)
		return (0);
	while (str && *str)
	{
		if (*str == '\'' || *str == '"')
			return (1);
		str++;
	}
	return (0);
}

int	is_empty(char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (*str != ' ')
			return (0);
		str++;
	}
	return (1);
}

int	has_pipe(char *str)
{
	while (*str)
	{
		str += skip_quote(str);
		if (*str == '|')
			return (1);
		str++;
	}
	return (0);
}
