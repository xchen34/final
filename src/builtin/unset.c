/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:56:11 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:56:13 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	minienv_del_next_node(t_env **node)
{
	t_env	*temp;

	temp = (*node)->next;
	(*node)->next = ((*node)->next)->next;
	free(temp->data);
	free(temp);
}

static void	remove_from_minienv(char *varname, t_env **minienv)
{
	t_env	*node;
	size_t	len;

	node = *minienv;
	len = ft_strlen(varname);
	if (ft_isdigit(ft_atoi(varname)))
	{
		while (*varname)
		{
			if (*varname == '_' || ft_isalpha(*varname))
				exit_varname_error_msg("unset", varname);
			varname++;
		}
	}
	else
	{
		while (node && node->next)
		{
			if (ft_strncmp((node->next)->data, varname, len) == 0)
				return (minienv_del_next_node(&node));
			node = node->next;
		}
	}
}

int	unset(char **args, t_env **minienv)
{
	char	*varname;
	char	option[3];

	args++;
	if (!*args)
		return (0);
	while (*args)
	{
		varname = *args;
		if (args[0] && args[0][0] == '-' && args[0][1] != '\0')
		{
			option[0] = args[0][0];
			option[1] = args[0][1];
			option[2] = '\0';
			print_error_msg(option, "invalid option", ERROR);
			return (2);
		}
		remove_from_minienv(varname, minienv);
		args++;
	}
	return (0);
}
