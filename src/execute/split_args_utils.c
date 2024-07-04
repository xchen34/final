/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:57:09 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:57:12 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	replace_single_quotes(char *str, char limiter)
{
	if (*str == limiter && *(str + 1) == limiter)
	{
		*(str + 1) = -2;
		*str = -2;
		str++;
	}
	while (*str && *str != limiter)
		str++;
	if (*str)
		str++;
	while (*str == limiter)
	{
		*(str - 1) = -2;
		*str = -2;
		str++;
	}
	if (*str)
		str++;
	if (*str)
		replace_single_quotes(str, limiter);
}

void	replace_double_quotes(char *str, char limiter)
{
	if (*str == limiter && *(str + 1) == limiter)
	{
		*(str + 1) = -3;
		*str = -3;
		str++;
	}
	while (*str && *str != limiter)
		str++;
	if (*str)
		str++;
	while (*str == limiter)
	{
		*(str - 1) = -3;
		*str = -3;
		str++;
	}
	if (*str)
		str++;
	if (*str)
		replace_double_quotes(str, limiter);
}

void	remove_quote(char *str)
{
	char	last_opened;

	last_opened = 0;
	while (*str && !last_opened)
	{
		if (*str == '\'' || *str == '"')
		{
			last_opened = *str;
			ft_memmove(str, str + 1, ft_strlen(str + 1) + 1);
		}
		else
			str++;
	}
	while (*str && last_opened)
	{
		if (*str && *str == last_opened)
		{
			last_opened = 0;
			ft_memmove(str, str + 1, ft_strlen(str + 1) + 1);
		}
		else
			str++;
	}
	if (*str)
		return (remove_quote(str));
}

void	replace_tabs_with_spaces(char *str)
{
	while (*str)
	{
		if (*str == '\t')
			*str = ' ';
		str++;
	}
}
