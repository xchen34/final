/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minienv_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:58:12 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:58:13 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*create_data(char *name, char *value)
{
	char	*data;
	int		size;
	int		i;

	size = ft_strlen(name) + ft_strlen(value) + 2;
	data = malloc(size * sizeof(char));
	i = 0;
	while (*name)
		data[i++] = *name++;
	data[i++] = '=';
	while (*value)
		data[i++] = *value++;
	data[i] = '\0';
	return (data);
}

void	minienv_update(char *name, char *value, t_env *minienv)
{
	t_env	*node;
	char	*new_data;
	int		size;
	int		i;

	node = minienv_node(name, minienv);
	if (!node)
	{
		new_data = create_data(name, value);
		list_append(new_data, &minienv);
		free(new_data);
		return ;
	}
	free(node->data);
	size = ft_strlen(name) + ft_strlen(value) + 2;
	new_data = malloc (size * sizeof(char));
	i = 0;
	while (*name)
		new_data[i++] = *name++;
	new_data[i++] = '=';
	while (*value)
		new_data[i++] = *value++;
	new_data[i++] = '\0';
	node->data = new_data;
}

static size_t	minienv_size(t_env *minienv)
{
	size_t	size;
	t_env	*node;

	size = 0;
	node = minienv;
	while (node)
	{
		size++;
		node = node->next;
	}
	return (size);
}

char	**minienv_to_env(t_env *minienv)
{
	char	**env;
	t_env	*node;
	int		i;

	env = malloc(sizeof(char *) * (minienv_size(minienv) + 1));
	node = minienv;
	i = 0;
	while (node)
	{
		env[i] = ft_strdup(node->data);
		i++;
		node = node->next;
	}
	env[i] = NULL;
	return (env);
}
