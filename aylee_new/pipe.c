/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 15:40:32 by aylee             #+#    #+#             */
/*   Updated: 2026/02/24 18:12:08 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * 파이프 없는 단일 명령어 처리
 * 리다이렉션 있으면 fork해서 fd 보호, 없으면 직접 실행
 *
 * 리다이렉션 시 fork하는 이유:
 * dup2로 stdin/stdout을 바꾸면 부모 프로세스 fd가 영구적으로 바뀌기 때문.
 * 자식에서 바꾸고 exit하면 부모의 fd는 그대로 유지됨.
 */
int	no_pipe(t_data *data, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	if (!cmd->redir)
		return (execute_command(data, cmd->argv));
	pid = fork();
	if (pid == -1)
		return (print_error(data, "fork", errno, 1), 1);
	if (pid == 0)
	{
		if (prepare_heredoc(data, cmd) == -1)
			exit(1);
		if (apply_redir(data, cmd->redir) == -1)
			exit(1);
		exit(execute_command(data, cmd->argv));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	return (data->exit_status);
}

/*
 * 각 명령어를 fork하여 자식 프로세스 생성
 * pids는 pipeline->pids에 저장 (init_pipes에서 이미 malloc됨)
 */
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
			exec_child(data, cur, pipeline, i);
		cur = cur->next;
		i++;
	}
	close_all_pipes(pipeline->pipes, pipeline->count - 1);
	return (0);
}

/*
 * 모든 자식 프로세스 대기
 * 마지막 명령어의 exit_status를 data에 저장
 */
int	wait_pids(t_data *data, t_pipes *pipeline)
{
	int	i;
	int	status;

	i = 0;
	while (i < pipeline->count)
	{
		waitpid(pipeline->pids[i], &status, 0);
		if (i == pipeline->count - 1)
		{
			if (WIFEXITED(status))
				data->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				data->exit_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (data->exit_status);
}

/*
 * pipeline 구조체 내 동적 메모리 해제
 */
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