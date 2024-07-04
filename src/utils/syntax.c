/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:59:32 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:59:33 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	syntax_error(char *token)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("syntax error near unexpected token '", STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putstr_fd("\'", STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

static int	unexpected_token(char *input)
{
	if (input[0] == '<' && input[1] == '<')
		syntax_error("<<");
	else if (input[0] == '>' && input[1] == '<')
		syntax_error(">>");
	input[1] = '\0';
	syntax_error(input);
	return (1);
}

static int	redirect_without_label(char *input)
{
	char	*redirect_pos;
	char	next_redirect;

	next_redirect = get_next_redirect(input);
	redirect_pos = get_redirect_position(input, next_redirect);
	if (!redirect_pos)
		return (0);
	if (redirect_pos[0] == '<' && redirect_pos[1] == '<')
		redirect_pos++;
	else if (redirect_pos[0] == '>' && redirect_pos[1] == '>')
		redirect_pos++;
	redirect_pos++;
	while (*redirect_pos == ' ' || *redirect_pos == '\t')
		redirect_pos++;
	if (*redirect_pos == '\0')
	{
		syntax_error("newline");
		return (1);
	}
	if (*redirect_pos == '|' || *redirect_pos == '>' || *redirect_pos == '<')
		return (unexpected_token(redirect_pos));
	return (redirect_without_label(redirect_pos));
}

int	is_invalid_syntax(char *input)
{
	if (input[0] == '|')
	{
		syntax_error("|");
		return (1);
	}
	if (redirect_without_label(input))
		return (1);
	if (has_empty_pipe(input))
		return (1);
	return (0);
}
