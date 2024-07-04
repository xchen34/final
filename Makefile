#EXECUTE
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LDLIBS	= -lreadline -lft

#PATH
LIBFT_PATH = ./libft
LIBS_PATH = -L$(LIBFT_PATH)
OBJ_DIR	= obj

#SRCS
BUILTIN = cd.c echo.c env.c exit.c export.c pwd.c unset.c
EXECUTE = exec_builtin.c exec_external.c exec_multi_cmd.c exec_one_cmd.c \
		get_path.c multi_cmds_utils.c split_args.c split_cmds.c split_args_utils.c \
		tilde.c wait.c
EXPAND = exit_status.c variable.c handle_expansion.c
MINIENV = minienv_str.c minienv_utils.c minienv.c
UTILS = error.c fd.c heredoc.c label_name.c pipe_quote.c redirect.c signal.c str.c syntax.c

SRCS = src/main.c src/prompt.c $(BUILTIN:%=src/builtin/%) $(EXECUTE:%=src/execute/%) \
	   $(EXPAND:%=src/expand/%) $(MINIENV:%=src/minienv/%) $(UTILS:%=src/utils/%)

NAME = minishell

OBJS = $(SRCS:src/%.c=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS) minishell.h 
	make -C $(LIBFT_PATH)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDLIBS) $(LIBS_PATH)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	make clean -C $(LIBFT_PATH)
	rm -rf $(OBJ_DIR)
	rm -f $(OBJ)

fclean: clean
	make fclean -C $(LIBFT_PATH)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
