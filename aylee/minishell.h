/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 00:00:00 by aylee             #+#    #+#             */
/*   Updated: 2026/03/02 19:57:35 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/libft.h"

extern volatile sig_atomic_t	g_signal;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
	t_env	*env;
	int		exit_status;
}	t_data;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	REDIR_PIPE
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	int				fd;
	int				quoted;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			*cmd;
	char			**argv;
	t_redir			*redir;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_pipes
{
	int		**pipes;
	int		count;
	pid_t	*pids;
}	t_pipes;

// env.c
t_env	*create_env_node(const char *key, const char *value);
void	free_env_node(t_env *node);
t_env	*find_env_node(t_env *head, const char *key);
t_env	*add_env_node(t_env **head, const char *key, const char *value);
t_env	*parse_env(char **envp);
void	free_env_list(t_env *head);
void	delete_env(t_env **head, char *key);
void	print_env_list(t_env *head);

// built_in.c
int		builtin_echo(t_data *data, char **args);
int		builtin_cd(t_data *data, char **args);
int		builtin_pwd(t_data *data);
int		builtin_export(t_data *data, char **args);
int		builtin_unset(t_data *data, char **args);
int		builtin_exit(t_data *data, char **args);
int		builtin_env(t_data *data);
int		execute_builtin(t_data *data, t_cmd *cmd);
int		set_env_var(t_data *data, const char *key, const char *value);
void	make_env_new(t_data *data, char **args, char *equal_sign, int i);

// utils.c
void	clean_up(t_data *data);
void	free_split(char **split);;
char	**env_to_array(t_env *env);

// main.c
void	print_error(t_data *data, char *cmd, int err_num, int exit_code);
void	print_error_msg(t_data *data, char *cmd, char *msg, int exit_code);
int		is_builtin(char *cmd);

// main_init.c
t_data	*init_data(char **envp);

// exec.c
char	*find_command_path(char *cmd, t_env *env);
int		execute_command(t_data *data, t_cmd *cmd);
int		execute_pipeline(t_data *data, t_cmd *cmd);
int		apply_redir(t_data *data, t_redir *redir);
void	exec_child(t_data *data, t_cmd *cmd, t_pipes *pipeline, int i);
void	close_all_pipes(int **pipes, int count);
int		count_cmd(t_cmd *cmd);
void	init_pipes(t_data *data, t_cmd *cmd, t_pipes *pipeline);
char	**get_execve_args(t_cmd *cmd);

// heredoc.c
void	heredoc_child(int write_fd, char *delim);
int		collect_heredoc(t_redir *redir);

// pipe.c
int		no_pipe(t_data *data, t_cmd *cmd);
int		get_pids(t_data *data, t_cmd *cmd, t_pipes *pipeline);
int		wait_pids(t_data *data, t_pipes *pipeline);
void	free_pipeline(t_pipes *pipeline);

// parse/
typedef enum e_tok_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APPEND,
	TOK_HEREDOC,
}	t_tok_type;

typedef struct s_token
{
	t_tok_type		type;
	char			*str;
	int				quoted;
	struct s_token	*next;
}	t_token;

t_cmd	*parse_pipeline(char *input, t_data *data);
void	free_cmd_list(t_cmd *cmd);
t_cmd	*new_cmd(void);
void	free_redir_list(t_redir *redir);
t_token	*lexer(char *input, t_data *data);
void	free_tokens(t_token *head);
t_token	*new_token(t_tok_type type, char *str);
char	*str_append(char *s, char *add);
char	*str_append_char(char *s, char c);
char	*expand_dollar(char *input, int *i, t_data *data);
char	*expand_heredoc_line(char *line, t_data *data);
void	signal_interactive(void);

// exec2.c
int		make_right_path(const char *cmd, char **path_dirs, char **full_path);
int		prepare_heredoc(t_data *data, t_cmd *cmd);
int		collect_heredoc(t_redir *redir);
int		count_heredocs(t_cmd *cmd);
void	signal_in_message(int line_count, char *delim);
int		collect_heredoc_fork(t_redir *redir, char *delim);

// image.c
void	shell_init(void);

#endif
