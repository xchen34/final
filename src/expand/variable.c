/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:58:00 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:58:02 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static	char	*find_var_position(char *input)
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
				if (*input == '$' && (ft_isalnum(input[1]) || input[1] == '_'))
					return (input);
				input++;
			}
		}
		if (*input == '$' && (ft_isalnum(input[1]) || input[1] == '_'))
			return (input);
		input++;
	}
	return (NULL);
}

static char	*add_quote_to_token(char *new_var_value, char *var_value)
{
	if (ft_strcmp(var_value, ">") == 0)
		ft_memcpy(new_var_value, "\">\"", 4);
	else if (ft_strcmp(var_value, ">>") == 0)
		ft_memcpy(new_var_value, "\">>\"", 5);
	else if (ft_strcmp(var_value, "<") == 0)
		ft_memcpy(new_var_value, "\"<\"", 4);
	else if (ft_strcmp(var_value, "<<") == 0)
		ft_memcpy(new_var_value, "\"<<\"", 5);
	else if (ft_strcmp(var_value, "|") == 0)
		ft_memcpy(new_var_value, "\"|\"", 4);
	else
		new_var_value = var_value;
	return (new_var_value);
}

static void	update_input(char **input, char *var_value, char *second_part)
{
	char	*first_part;
	char	*updated_input;
	char	*new_var_value;

	new_var_value = malloc(5 * sizeof(char));
	if (!new_var_value)
		exit_error("Memory allocation", "failed", 1, 1);
	if (var_value)
		var_value = add_quote_to_token(new_var_value, var_value);
	if (!*input[0] && !var_value)
		first_part = ft_strdup("");
	else if (!*input[0] && var_value)
		first_part = ft_strdup(var_value);
	else if (!var_value)
		first_part = ft_strdup(*input);
	else
		first_part = ft_strjoin(*input, var_value);
	updated_input = ft_strjoin(first_part, second_part);
	free(first_part);
	free(*input);
	free(new_var_value);
	*input = updated_input;
}

void	expand_variable(char **input, t_env *minienv)
{
	char	*var_position;
	char	*var_name;
	char	*var_value;
	int		name_size;

	var_position = find_var_position(*input);
	if (var_position)
	{
		name_size = 0;
		if (ft_isdigit(var_position[1]) && ft_isalnum(var_position[2]))
			name_size++;
		else
		{
			while (ft_isalnum(var_position[name_size + 1])
				|| var_position[name_size + 1] == '_')
				name_size++;
		}
		var_name = ft_substr(var_position, 1, name_size);
		*var_position = '\0';
		var_value = minienv_value(var_name, minienv);
		update_input(input, var_value, (var_position + 1 + name_size));
		free(var_name);
		expand_variable(input, minienv);
	}
}

int	is_valid_varname(char *name, int is_sign_plus)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name) - 1;
	if (!ft_isalpha(*name) && *name != '_')
		return (0);
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
		{
			if (is_sign_plus == 1 && i == len && name[i] == '+')
				return (1);
			return (0);
		}
		i++;
	}
	return (1);
}
