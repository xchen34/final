/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:57:53 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:57:54 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_expansion(char **input, t_env *minienv, int exit_status)
{
	expand_exit_status(input, exit_status);
	expand_variable(input, minienv);
}
