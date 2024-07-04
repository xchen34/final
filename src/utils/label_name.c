/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   label_name.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:58:57 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:58:59 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	get_name_end(char *name)
{
	int	i;

	i = 0;
	while (name[i] && name[i] != ' ' && name[i] != '>' && name[i] != '<'
		&& name[i] != '|' && name[i] != '\t')
	{
		if (name[i] == '\'')
		{
			ft_memmove(&name[i], &name[i + 1], ft_strlen(&name[i + 1]) + 1);
			while (name[i] && name[i] != '\'')
				i++;
			ft_memmove(&name[i], &name[i + 1], ft_strlen(&name[i + 1]) + 1);
		}
		else if (name[i] == '"')
		{
			ft_memmove(&name[i], &name[i + 1], ft_strlen(&name[i + 1]) + 1);
			while (name[i] && name[i] != '"')
				i++;
			ft_memmove(&name[i], &name[i + 1], ft_strlen(&name[i + 1]) + 1);
		}
		else if (name[i] && name[i] != ' ' && name[i] != '>' && name[i] != '<'
			&& name[i] != '|' && name[i] != '\t')
			i++;
	}
	return (i);
}

char	*get_label_name(char *redirect_str)
{
	int		start;
	int		end;
	int		len;
	char	*name;
	char	*remaining_cmd;

	start = 0;
	len = ft_strlen(&redirect_str[start + 1]) + 1;
	ft_memmove(redirect_str, redirect_str + 1, ft_strlen(redirect_str + 1) + 1);
	if (redirect_str[start] == '>')
		ft_memmove(&redirect_str[start], &redirect_str[start + 1], len);
	while (redirect_str[start] == ' ' || redirect_str[start] == '\t')
		start++;
	end = get_name_end(&redirect_str[start]);
	name = ft_substr(&redirect_str[start], 0, end);
	remaining_cmd = &redirect_str[start + end];
	ft_memcpy(redirect_str, remaining_cmd, ft_strlen(remaining_cmd) + 2);
	return (name);
}
