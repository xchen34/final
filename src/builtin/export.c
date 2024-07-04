/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:56:02 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:56:04 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	declare_env(t_env *minienv)
{
	t_env	*node;
	char	*varname;

	node = minienv;
	while (node)
	{
		varname = name_only(node->data);
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(varname, STDOUT_FILENO);
		if (ft_strchr(node->data, '='))
		{
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
			ft_putstr_fd(value_only(node->data), STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		ft_putstr_fd("\n", STDOUT_FILENO);
		free(varname);
		node = node->next;
	}
	return (0);
}

static void	export_plus_sign_second_part(char *data, char *varname,
	char *value, t_env **minienv)
{
	char	*new_entry;
	char	*equal_sign_pos;
	size_t	len;

	if (!minienv_node(varname, *minienv))
	{
		equal_sign_pos = ft_strchr(data, '=');
		len = ft_strlen(varname) + ft_strlen(equal_sign_pos) + 1;
		new_entry = malloc(len + 1);
		if (!new_entry)
			return ;
		ft_strcpy(new_entry, varname);
		ft_strcat(new_entry, equal_sign_pos);
		list_append(new_entry, minienv);
		free(new_entry);
	}
	else if (minienv_node(varname, *minienv) && !value)
	{
		value = value_only(data);
		minienv_update(varname, value, *minienv);
	}
}

static int	export_plus_sign(char *data, char *varname, t_env **minienv)
{
	char	*value;
	char	*new_value;
	size_t	new_value_len;

	varname = name_only_with_plus_sign(data, varname);
	if (!varname)
		return (EXIT_FAILURE);
	value = minienv_value(varname, *minienv);
	if (minienv_node(varname, *minienv) && value)
	{
		new_value_len = ft_strlen(ft_strchr(data, '+') + 2) + 1;
		new_value = malloc(ft_strlen(value) + new_value_len);
		if (!new_value)
			return (free(varname), EXIT_FAILURE);
		ft_strcpy(new_value, value);
		ft_strcat(new_value, ft_strchr(data, '+') + 2);
		minienv_update(varname, new_value, *minienv);
		free(new_value);
	}
	else
		export_plus_sign_second_part(data, varname, value, minienv);
	free(varname);
	return (0);
}

static int	export_expansion(char **args, t_env **minienv)
{
	int		exit_status;
	char	*varname;
	char	*data;

	exit_status = 0;
	while (*args)
	{
		data = *args;
		varname = name_only(data);
		if (is_valid_varname(varname, 1) && ft_strchr(varname, '+')
			&& ft_strchr(data, '+')[1] == '=')
			exit_status = export_plus_sign(data, varname, minienv);
		else if (!is_valid_varname(varname, 0))
		{
			exit_varname_error_msg("export", data);
			exit_status = EXIT_FAILURE;
		}
		else if (!minienv_node(varname, *minienv))
			list_append(data, minienv);
		else if (minienv_node(varname, *minienv) && value_only(data))
			minienv_update(varname, value_only(data), *minienv);
		free(varname);
		args++;
	}
	return (exit_status);
}

int	export(char **args, t_env **minienv)
{
	int		exit_status;
	char	option[3];

	args++;
	exit_status = 0;
	if (!*args)
		return (declare_env(*minienv));
	if (args[0] && args[0][0] == '-' && args[0][1] != '\0')
	{
		option[0] = args[0][0];
		option[1] = args[0][1];
		option[2] = '\0';
		print_error_msg(option, "invalid option", ERROR);
		return (2);
	}
	if (args[0] && ft_strchr(args[0], '!'))
	{
		print_error_msg(ft_strchr(args[0], '!'), "event not found", ERROR);
		return (1);
	}
	exit_status = export_expansion(args, minienv);
	return (exit_status);
}
