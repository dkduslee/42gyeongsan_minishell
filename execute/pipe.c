/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 15:40:32 by aylee             #+#    #+#             */
/*   Updated: 2026/03/23 13:49:09 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * 파이프 없는 단일 명령어 처리
 * builtin 중 상태를 변경하는 것(cd, export, unset, exit)은
 * 부모 프로세스에서 직접 실행해야 함.
 * 리다이렉션이 있으면 fork하여 부모 fd를 보호.
 */
static void	no_pipe_child(t_data *data, t_cmd *cmd)
{
	int	status;

	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (prepare_heredoc(data, cmd) == -1)
	{
		rl_clear_history();
		free_cmd_list(cmd);
		clean_up(data, NULL);
		exit(1);
	}
	if (apply_redir(data, cmd->redir) == -1)
	{
		rl_clear_history();
		free_cmd_list(cmd);
		clean_up(data, NULL);
		exit(1);
	}
	status = execute_command(data, cmd);
	rl_clear_history();
	free_cmd_list(cmd);
	free_env_list(data->env);
	exit(status);
}

int	no_pipe(t_data *data, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	if (is_builtin(cmd->cmd))
	{
		if (!cmd->redir)
			return (data->exit_status = execute_builtin(data, cmd));
	}
	else if (!cmd->redir)
		return (execute_command(data, cmd));
	pid = fork();
	if (pid == -1)
		return (print_error(data, "fork", errno, 1), 1);
	if (pid == 0)
		no_pipe_child(data, cmd);
	waitpid(pid, &status, 0);
	update_exit_status(data, status);
	return (data->exit_status);
}

int	get_pids(t_data *data, t_cmd *cmd, t_pipes *pipeline)
{
	t_cmd	*cur;
	int		i;

	cur = cmd;
	i = 0;
	while (cur)
	{
		pipeline->pids[i] = fork();
		if (pipeline->pids[i] == -1)
		{
			print_error(data, "fork", errno, 1);
			close_all_pipes(pipeline->pipes, pipeline->count - 1);
			return (1);
		}
		if (pipeline->pids[i] == 0)
			exec_child(data, cmd, pipeline, i);
		cur = cur->next;
		i++;
	}
	close_all_pipes(pipeline->pipes, pipeline->count - 1);
	return (0);
}

int	wait_pids(t_data *data, t_pipes *pipeline)
{
	int	i;
	int	status;

	i = 0;
	while (i < pipeline->count)
	{
		waitpid(pipeline->pids[i], &status, 0);
		if (i == pipeline->count - 1)
			update_exit_status(data, status);
		i++;
	}
	return (data->exit_status);
}

void	free_pipeline(t_pipes *pipeline)
{
	int	i;

	if (pipeline->pipes)
	{
		i = 0;
		while (i < pipeline->count - 1)
			free(pipeline->pipes[i++]);
		free(pipeline->pipes);
		pipeline->pipes = NULL;
	}
	if (pipeline->pids)
	{
		free(pipeline->pids);
		pipeline->pids = NULL;
	}
}
