/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:00:00 by aylee             #+#    #+#             */
/*   Updated: 2026/02/24 18:56:35 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command_path(const char *cmd, t_env *env)
{
	t_env	*path_node;
	char	**dirs;
	char	*full_path;

	full_path = NULL;
	if (ft_strchr(cmd, '/')) //기본적으로 경로를 가진 것으로 판단.
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_node = find_env_node(env, "PATH"); //path 환경변수
	if (!path_node || !path_node->value)
		return (NULL);
	dirs = ft_split(path_node->value, ':'); //경로들을 따로 분리.
	if (!dirs)
		return (NULL);
	if (make_right_path(cmd, dirs, &full_path))
		return (free_split(dirs), full_path);
	return (free_split(dirs), NULL);
}

//히어독만의 필드 (임시저장 혹은 파이프)를 따로 구조체 안에 넣는 것이 중요한가?
//히어독은 pipe를 생성하고 부모가 write 쪽에 내용 입력, 자식은 read 쪽을 듑.
int	apply_redir(t_data *data, t_redir *redir)
{
	int	fd;

	while (redir)
	{
		if (redir->type == REDIR_IN) //읽기 하고 듑
			fd = open(redir->file, O_RDONLY);
		else if (redir->type == REDIR_OUT) // 쓰기로 듑
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == REDIR_APPEND) // 쓰기 듑
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1) //실패의 경우 /////////////인데? 히어독은 어떻게 판별을 하지?
		{
			print_error(data, redir->file, errno, 1);
			return (-1);
		}
		if (redir->type == REDIR_IN)
			dup2(fd, STDIN_FILENO);
		else if (redir->type == REDIR_HEREDOC) //히어독 사전에 준비해둔 파트대로 연결함.
		{
			dup2(redir->fd, STDIN_FILENO);
			close(redir->fd);
		}
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return (0);
}

void	close_all_pipes(int **pipes, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

int	count_cmd(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}
//t_pipes pipeline;
void	exec_child(t_data *data, t_cmd *cmd, t_pipes *pipeline, int i)
{
	char	*cmd_path;
	char	**envp;

	if (i > 0) //첫번째가 아니면 그전꺼로 읽어옴.
		dup2(pipeline->pipes[i - 1][0], STDIN_FILENO);
	if (i < pipeline->count - 1) //맨 끝 아니면
		dup2(pipeline->pipes[i][1], STDOUT_FILENO);
	close_all_pipes(pipeline->pipes, pipeline->count - 1);
	if (prepare_heredoc(data, cmd) == -1)
		exit(1);
	if (apply_redir(data, cmd->redir) == -1)
		exit(1);
	if (is_builtin(cmd->argv[0]))
		exit(execute_builtin(data, cmd->argv));
	cmd_path = find_command_path(cmd->argv[0], data->env);
	if (!cmd_path)
	{
		print_error_msg(data, cmd->argv[0], "command not found", 127);
		exit(127);
	}
	envp = env_to_array(data->env);
	execve(cmd_path, cmd->argv, envp);
	print_error(data, cmd->argv[0], errno, 126);
	exit(126);
}

int	execute_command(t_data *data, char **args)
{
	char	*cmd_path;
	char	**envp;
	pid_t	pid;
	int		status;

	if (!args || !args[0])
		return (0);
	if (is_builtin(args[0]))
		return (execute_builtin(data, args));
	cmd_path = find_command_path(args[0], data->env);
	if (!cmd_path)
	{
		print_error_msg(data, args[0], "command not found", 127);
		return (127);
	}
	envp = env_to_array(data->env);
	pid = fork();
	if (pid == -1)
	{
		print_error(data, "fork", errno, 1);
		free(cmd_path);
		free_split(envp);
		return (1);
	}
	if (pid == 0)
	{
		execve(cmd_path, args, envp);
		print_error(data, args[0], errno, 126);
		exit(126);
	}
	waitpid(pid, &status, 0);
	free(cmd_path);
	free_split(envp);
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->exit_status = 128 + WTERMSIG(status);
	return (data->exit_status);
}

/*
 * pipe fd 배열 초기화
 * 실패 시 pipeline->pipes = NULL로 설정
 */
void	init_pipes(t_data *data, t_cmd *cmd, t_pipes *pipeline)
{
	int	i;

	pipeline->count = count_cmd(cmd);
	pipeline->pids = malloc(sizeof(pid_t) * pipeline->count);
	pipeline->pipes = malloc(sizeof(int *) * (pipeline->count - 1));
	if (!pipeline->pipes || !pipeline->pids)
	{
		print_error(data, "malloc", errno, 1);
		pipeline->pipes = NULL;
		return ;
	}
	i = 0;
	while (i < pipeline->count - 1)
	{
		pipeline->pipes[i] = malloc(sizeof(int) * 2);
		if (!pipeline->pipes[i] || pipe(pipeline->pipes[i]) == -1)
		{
			print_error(data, "pipe", errno, 1);
			close_all_pipes(pipeline->pipes, i);
			free(pipeline->pipes);
			pipeline->pipes = NULL;
			return ;
		}
		i++;
	}
}

int	execute_pipeline(t_data *data, t_cmd *cmd)
{
	t_pipes	pipeline;

	if (!cmd || !cmd->argv)
		return (1);
	if (!cmd->next)
		return (no_pipe(data, cmd));
	init_pipes(data, cmd, &pipeline);
	if (!pipeline.pipes)
		return (1);
	if (get_pids(data, cmd, &pipeline))
	{
		free_pipeline(&pipeline);
		return (1);
	}
	wait_pids(data, &pipeline);
	free_pipeline(&pipeline);
	return (data->exit_status);
}