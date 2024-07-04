/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leochen <leochen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:59:53 by leochen           #+#    #+#             */
/*   Updated: 2024/07/04 17:59:55 by leochen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define ERROR 0
# define PERROR 1
# define FAILED 0
# define SUCCESS 1

# include "libft/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>  // history
# include <readline/readline.h> // readline
# include <signal.h>            // sigaction
# include <stdlib.h>
# include <sys/stat.h> //stat
# include <sys/wait.h> //wait
# include <unistd.h>

# define BUF_SIZE 2048
# define MISUSE 2
# define BOLD "\001\e[1m\002"
# define RESET_SIZE "\001\e[0m\002"
# define GREEN "\001\e[0;32m\002"
# define YELLOW "\001\e[0;93m\002"
# define BLUE "\001\e[0;34m\002"
# define PURPLE "\001\e[0;35m\002"
# define CYAN "\001\e[0;96m\002"
# define WHITE "\001\e[0;37m\002"
# define RESET "\001\e[0m\002"
# define TRUE 1
# define FALSE 0

typedef struct s_env
{
	char			*data;
	struct s_env	*next;
}					t_env;

// main.c
void				free_array(char **arr);
int					minishell(t_env *minienv);
char				*do_prompt(t_env *minienv);

// builtin
int					cd(char **args, t_env *minienv);
int					echo(char **args);
int					env(char **args, t_env *minienv);
int					export(char **args, t_env **minienv);
int					pwd(t_env *minienv);
int					unset(char **args, t_env **minienv);
int					builtin_exit(char **args, t_env **minienv);

// execute
int					is_builtin(char *cmd);
int					exec_builtin(char **args, t_env **minienv);
int					exec_external(char **args, t_env *minienv);
int					exec_one_cmd(char *cmd, t_env **minienv);
char				*get_path(char *cmd, t_env *minienv);
int					exec_multi_cmds(char **cmds, t_env **minienv);
int					*init_children_pid(char **cmds);
void				handle_pipe(int fd_out, char *current_cmd, char **cmds);
void				replace_single_quotes(char *str, char limiter);
void				replace_double_quotes(char *str, char limiter);
void				remove_quote(char *str);
void				replace_tabs_with_spaces(char *str);
char				**split_args(char *cmd, t_env **minienv);
char				*find_and_replace_tilde(char *cmd, t_env *minienv);
char				**split_cmds(char *input);
int					wait_child(int child_pid, int is_last_child);
int					wait_children(int children_pid[1024]);

// expand
void				expand_exit_status(char **input, int exit_status);
void				expand_variable(char **input, t_env *minienv);
int					is_valid_varname(char *name, int sign);
void				handle_expansion(char **input, t_env *minienv,
						int exit_status);

// minienv
void				list_append(char *data, t_env **list);
t_env				*init_minienv(char **env);
void				free_minienv(t_env **minienv);
t_env				*minienv_node(char *name, t_env *minienv);
char				*minienv_value(char *name, t_env *minienv);
void				minienv_update(char *name, char *value, t_env *minienv);
char				**minienv_to_env(t_env *minienv);
char				*value_only(char *data);
char				*name_only(char *data);
char				*name_only_with_plus_sign(char *varname, char *data);

// utils
void				print_error_msg(char *cmd, char *msg, int print_error);
void				exit_error(char *cmd, char *msg, int error,
						int print_error);
int					has_input_error(char *input, int *exit_status,
						t_env *minienv);
int					exit_varname_error_msg(char *cmd, char *varname);
void				redirect_fd(int fd_to_redirect, int fd_location);
void				close_extra_fds(void);
void				close_all_fds(void);
char				*tmp_filename(int heredoc_nbr);
int					handle_heredoc(char *input, int *exit_status,
						t_env *minienv);
char				*get_label_name(char *redirect_str);
int					skip_quote(char *str);
int					has_empty_pipe(char *input);
int					has_unclosed_quote(char *str);
char				get_next_redirect(char *str);
char				*get_redirect_position(char *str, char redirect_char);
int					redirect_input(char *cmd);
int					redirect_output(char *cmd);
void				redirect_heredoc(char *cmd, int heredoc_nbr);
void				define_main_signal(void);
void				define_heredoc_signal(int child_pid);
void				define_exec_signal(int child_pid);
int					contains_quote_mark(char *str);
int					contains_alpha(char *args);
int					is_empty(char *str);
int					has_pipe(char *str);
void				syntax_error(char *token);
int					is_invalid_syntax(char *input);

// bonus
void				expand_wildcard(char *search, char **input, t_env *minienv);
char				*get_wildcard_value(char *wildcard_label, char **input,
						t_env *minienv);

#endif