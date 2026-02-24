/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 00:00:00 by aylee             #+#    #+#             */
/*   Updated: 2026/02/24 18:12:06 by aylee            ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/libft.h"

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
	int				fd; //히어독용 필드
	char			*delimiter; //히어독용 필드
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			*cmd;
	char			**argv;
	//quote를 표시하는 flag 부분
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
t_data	*init_data(char **envp);

// built_in.c
int		builtin_echo(t_data *data, char **args);
int		builtin_cd(t_data *data, char **args);
int		builtin_pwd(void);
int		builtin_export(t_data *data, char **args);
int		builtin_unset(t_data *data, char **args);
int		builtin_exit(t_data *data, char **args);
int		builtin_env(t_data *data);
int		execute_builtin(t_data *data, char **args);
int		set_env_var(t_data *data, const char *key, const char *value);

// utils.c
int	ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
size_t	ft_strlen(const char *s);
int		ft_atoi(const char *str);
void	clean_up(t_data *data);
char	**ft_split(char const *s, char c);
void	free_split(char **split);
size_t	ft_strlcat(char *dest, const char *src, size_t size);
char	*ft_strjoin(char const *s1, char const *s2);
char	**env_to_array(t_env *env);

// main_init
void	print_error(t_data *data, char *cmd, int err_num, int exit_code);
void	print_error_msg(t_data *data, char *cmd, char *msg, int exit_code);
int		is_builtin(char *cmd);
t_data	*init_data(char **envp);

// exec.c
char	*find_command_path(const char *cmd, t_env *env);
int		execute_command(t_data *data, char **args);
int		execute_pipeline(t_data *data, t_cmd *cmd);
int		apply_redir(t_data *data, t_redir *redir);
void	exec_child(t_data *data, t_cmd *cmd, t_pipes *pipeline, int i);
void	close_all_pipes(int **pipes, int count);
int		count_cmd(t_cmd *cmd);
void	init_pipes(t_data *data, t_cmd *cmd, t_pipes *pipeline);

// pipe.c
int		no_pipe(t_data *data, t_cmd *cmd);
int		get_pids(t_data *data, t_cmd *cmd, t_pipes *pipeline);
int		wait_pids(t_data *data, t_pipes *pipeline);
void	free_pipeline(t_pipes *pipeline);

// parse.c
t_cmd	*parse_pipeline(char *input);
void	free_cmd_list(t_cmd *cmd);

// exec2.c
int	make_right_path(const char *cmd, char **path_dirs, char **full_path);
int	prepare_heredoc(t_data *data, t_cmd *cmd);
int collect_heredoc(t_redir *redir);
int count_heredocs(t_cmd *cmd);

// image.c
void	shell_init(void);

#endif
