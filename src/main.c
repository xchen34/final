/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:26:18 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 18:01:55 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
	arr = NULL;
}

static int check_if_input_is_random()
{
    struct stat input_stat;
    struct stat random_stat;

    if (fstat(STDIN_FILENO, &input_stat) < 0) {
        perror("Failed to get stat for standard input");
        return -1;
    }
    if (stat("/dev/random", &random_stat) < 0) {
        perror("Failed to get stat for /dev/random");
        return -1;
    }
    if (input_stat.st_dev == random_stat.st_dev && input_stat.st_ino == random_stat.st_ino) {
        fprintf(stderr, "Error: Input is from /dev/random, which is not supported.\n");
        return 1;
    }

    return 0;
}

int	minishell(t_env *minienv)
{
	int		exit_status;
	char	*input;
	char	**cmds;

	exit_status = EXIT_SUCCESS;
	while (1)
	{
		define_main_signal();
		input = do_prompt(minienv);
        if (check_if_input_is_random() == 1) {
            ft_putstr_fd("Error: Input from /dev/random is not allowed.\n", 2);
            free(input);
            continue;
        }
		if (has_input_error(input, &exit_status, minienv))
			continue ;
		handle_expansion(&input, minienv, exit_status);
		if (!has_pipe(input))
			exit_status = exec_one_cmd(input, &minienv);
		else
		{
			cmds = split_cmds(input);
			free(input);
			exit_status = exec_multi_cmds(cmds, &minienv);
			free_array(cmds);
		}
	}
	rl_clear_history();
	return (exit_status);
}

int	main(int ac, char **av, char **env)
{
	if (av && ac > 1)
	{
		ft_putstr_fd("Minishell can't take arguments.\n", STDOUT_FILENO);
		return (EXIT_FAILURE);
	}
	return (minishell(init_minienv(env)));
}
