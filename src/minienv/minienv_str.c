/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minienv_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:58:06 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:58:08 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_env	*minienv_node(char *name, t_env *minienv)
{
	t_env	*node;
	int		size;

	node = minienv;
	size = ft_strlen(name);
	while (node)
	{
		if (ft_strncmp(name, node->data, size) == 0)
		{
			if (node->data[size] == '=' || node->data[size] == '\0')
				return (node);
		}
		node = node->next;
	}
	return (NULL);
}

char	*minienv_value(char *name, t_env *minienv)
{
	t_env	*node;

	node = minienv_node(name, minienv);
	if (!node)
		return (NULL);
	return (value_only(node->data));
}

char	*value_only(char *data)
{
	int	i;

	i = 0;
	while (data[i] != '=' && data[i])
		i++;
	if (!data[i])
		return (NULL);
	return (&data[i + 1]);
}

char	*name_only(char *data)
{
	int	i;

	i = 0;
	while (data[i] && (data[i] != '=') && (data[i] != '+'))
		i++;
	if (data[i] == '+')
		return (ft_substr(data, 0, i + 1));
	return (ft_substr(data, 0, i));
}

char	*name_only_with_plus_sign(char *varname, char *data)
{
	char	*plus_sign_pos;

	plus_sign_pos = ft_strchr(data, '+');
	if (plus_sign_pos)
		varname = ft_substr(data, 0, plus_sign_pos - data);
	else
		varname = name_only(data);
	if (!varname)
		return (NULL);
	return (varname);
}
