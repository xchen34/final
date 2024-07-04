/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minienv.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:58:26 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:58:27 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../minishell.h"

void	list_append(char *data, t_env **list)
{
	t_env	*new_node;
	t_env	*node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->data = ft_strdup(data);
	new_node->next = NULL;
	if (!*list)
	{
		*list = new_node;
		return ;
	}
	node = *list;
	while (node->next)
		node = node->next;
	node->next = new_node;
}

t_env	*init_minienv(char **env)
{
	t_env	*list;
	char	*home;
	int		i;

	list = NULL;
	i = 0;
	if (!env[0])
		return (NULL);
	while (env[i])
	{
		list_append(env[i], &list);
		i++;
	}
	if (!minienv_node("OLDPWD", list))
		list_append("OLDPWD", &list);
	home = ft_strjoin("__HOME=", minienv_value("HOME", list));
	list_append(home, &list);
	free(home);
	return (list);
}

void	free_minienv(t_env **minienv)
{
	t_env	*node;
	t_env	*next_node;

	node = *minienv;
	while (node)
	{
		free(node->data);
		next_node = node->next;
		free(node);
		node = next_node;
	}
	minienv = NULL;
}
