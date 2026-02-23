/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:00:00 by aylee             #+#    #+#             */
/*   Updated: 2026/02/23 14:03:59 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command_path(const char *cmd, t_env *env)
{
	t_env	*path_node;
	char	**dirs;
	char	*full_path;
	char	*tmp;
	int		i;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_node = find_env_node(env, "PATH");
	if (!path_node || !path_node->value)
		return (NULL);
	dirs = ft_split(path_node->value, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	full_path = NULL;
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
			break ;
		free(full_path);
		full_path = NULL;
		i++;
	}
	free_split(dirs);
	return (full_path);
}

static int	apply_redir(t_data *data, t_redir *redir)
{
	int	fd;

	while (redir)
	{
		if (redir->type == REDIR_IN)
			fd = open(redir->file, O_RDONLY);
		else if (redir->type == REDIR_OUT)
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == REDIR_APPEND)
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
		{
			redir = redir->next;
			continue ;
		}
		if (fd == -1)
		{
			print_error(data, redir->file, errno, 1);
			return (-1);
		}
		if (redir->type == REDIR_IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return (0);
}

static void	close_all_pipes(int **pipes, int count)
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

static void	exec_child(t_data *data, t_cmd *cmd,
				int **pipes, int i, int cmd_count)
{
	char	*cmd_path;
	char	**envp;

	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < cmd_count - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	close_all_pipes(pipes, cmd_count - 1);
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

static int	count_cmd(t_cmd *cmd)
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

int	execute_pipeline(t_data *data, t_cmd *cmd)
{
	int		cmd_count;
	int		**pipes;
	pid_t	*pids;
	t_cmd	*cur;
	int		i;
	int		status;
	pid_t	pid;

	if (!cmd || !cmd->argv)
		return (1);
	if (!cmd->next)
	{
		if (!cmd->redir)
			return (execute_command(data, cmd->argv));
		pid = fork();
		if (pid == 0)
		{
			if (apply_redir(data, cmd->redir) == -1)
				exit(1);
			exit(execute_command(data, cmd->argv));
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
		return (data->exit_status);
	}
	cmd_count = count_cmd(cmd);
	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	if (!pipes)
		return (print_error(data, "malloc", errno, 1), 1);
	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			print_error(data, "pipe", errno, 1);
			return (1);
		}
		i++;
	}
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return (print_error(data, "malloc", errno, 1), 1);
	cur = cmd;
	i = 0;
	while (cur)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			print_error(data, "fork", errno, 1);
			close_all_pipes(pipes, cmd_count - 1);
			free(pids);
			return (1);
		}
		if (pids[i] == 0)
			exec_child(data, cur, pipes, i, cmd_count);
		cur = cur->next;
		i++;
	}
	close_all_pipes(pipes, cmd_count - 1);
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				data->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				data->exit_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	i = 0;
	while (i < cmd_count - 1)
		free(pipes[i++]);
	free(pipes);
	free(pids);
	return (data->exit_status);
}