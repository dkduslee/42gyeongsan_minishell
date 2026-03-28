/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:00:00 by aylee             #+#    #+#             */
/*   Updated: 2026/03/26 19:25:42 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_fd_open(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == REDIR_IN)
		fd = open(redir->file, O_RDONLY);
	else if (redir->type == REDIR_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

int	apply_redir(t_data *data, t_redir *redir)
{
	int	fd;

	while (redir)
	{
		fd = set_fd_open(redir);
		if (redir->type == REDIR_HEREDOC)
		{
			dup2(redir->fd, STDIN_FILENO);
			close(redir->fd);
			redir->fd = -1;
			redir = redir->next;
			continue ;
		}
		if (fd == -1)
			return (print_error(data, redir->file, errno, 1), -1);
		if (redir->type == REDIR_IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return (0);
}

int	prepare_child(t_data *data, t_cmd *cmd, t_pipes *pipeline, int i)
{
	if (i > 0)
		dup2(pipeline->pipes[i - 1][0], STDIN_FILENO);
	if (i < pipeline->count - 1)
		dup2(pipeline->pipes[i][1], STDOUT_FILENO);
	close_all_pipes(pipeline->pipes, pipeline->count - 1);
	if (prepare_heredoc(data, cmd) == -1)
		return (-1);
	if (apply_redir(data, cmd->redir) == -1)
		return (-1);
	return (0);
}

void	exec_child(t_data *data, t_cmd *cmd, t_pipes *pipeline, int i)
{
	char	*cmd_path;
	char	**envp;
	char	**args;

	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (prepare_child(data, cmd, pipeline, i) == -1)
		exit(1);
	if (is_builtin(cmd->cmd))
		exit(execute_builtin(data, cmd));
	cmd_path = find_command_path(cmd->cmd, data->env);
	if (!cmd_path)
	{
		print_error_msg(data, cmd->cmd, "command not found", 127);
		exit(127);
	}
	envp = env_to_array(data->env);
	args = get_execve_args(cmd);
	execve(cmd_path, args, envp);
	print_error(data, cmd->cmd, errno, 126);
	free(cmd_path);
	free_split(args);
	free_split(envp);
	exit(126);
}

int	execute_pipeline(t_data *data, t_cmd *cmd)
{
	t_pipes	pipeline;

	if (!cmd || !cmd->cmd)
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
