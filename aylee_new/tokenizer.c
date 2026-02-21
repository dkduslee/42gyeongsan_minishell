/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 16:37:17 by aylee             #+#    #+#             */
/*   Updated: 2026/02/21 17:18:00 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static t_redir_type	get_type(char *str)
// {
// 	if (ft_strncmp(str, "|", 2) == 0)
// 		return (REDIR_PIPE);
// 	else if (ft_strncmp(str, "<<", 3) == 0)
// 		return (REDIR_HEREDOC);
// 	else if (ft_strncmp(str, ">>", 3) == 0)
// 		return (REDIR_APPEND);
// 	else if (ft_strncmp(str, "<", 2) == 0)
// 		return (REDIR_IN);
// 	else if (ft_strncmp(str, ">", 2) == 0)
// 		return (REDIR_OUT);
// 	return (WORD);
// }

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

/*
 * 리다이렉션 적용 함수 (실행부 담당)
 * redir 리스트를 순서대로 순회하며 fd를 dup2로 연결한다.
 * 실패 시 -1 반환, shell->exit_status = 1 설정
 */
static int	apply_redir(t_redir *redir, t_data *shell)
{
	int	fd;
	char	buf[256];

	while (redir)
	{
		if (redir->type == REDIR_IN)
			fd = open(redir->file, O_RDONLY);
		else if (redir->type == REDIR_OUT)
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == REDIR_APPEND)
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else // REDIR_HEREDOC: 실행부에서 pipe로 처리, 여기선 skip
		{
			redir = redir->next;
			continue ;
		}
		if (fd == -1)
		{
			// perror로 errno 기반 에러 출력
			ft_strlcpy(buf, "minishell: ", sizeof(buf));
			ft_strlcat(buf, redir->file, sizeof(buf));
			perror(buf);
			shell->exit_status = 1;
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

/*
 * 파이프 없는 단일 명령어 실행
 * 빌트인이면 직접 실행, 아니면 fork + execve
 */
static int	execute_single(t_data *shell, t_cmd *cmd)
{
	pid_t	pid;
	int	status;
	char	*cmd_path;
	char	buf[256];

	// argv[0]이 NULL이면 실행할 것 없음
	if (!cmd->argv || !cmd->argv[0])
		return (0);
	if (is_builtin(cmd->argv[0]))
		return (execute_builtin(shell, cmd->argv));
	cmd_path = find_command_path(cmd->argv[0], shell->envp);
	if (!cmd_path)
	{
		// errno 없는 에러 → write + strerror 조합
		write(2, "minishell: ", 11);
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
		write(2, ": command not found\n", 20);
		shell->exit_status = 127;
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (-1);
	}
	if (pid == 0)
	{
		if (apply_redir(cmd->redir, shell) == -1)
			exit(1);
		execve(cmd_path, cmd->argv, shell->envp);
		// execve 실패: errno 설정됨 → perror
		ft_strlcpy(buf, "minishell: ", sizeof(buf));
		ft_strlcat(buf, cmd->argv[0], sizeof(buf));
		perror(buf);
		exit(126);
	}
	waitpid(pid, &status, 0);
	if (cmd_path != cmd->argv[0])
		free(cmd_path);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	return (shell->exit_status);
}

/*
 * 파이프 닫기 헬퍼
 */
static void	close_pipes(int **pipes, int count)
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

/*
 * execute() — md 명세 4.2의 메인 실행 함수
 * t_cmd 파이프라인 리스트를 받아 순서대로 실행한다.
 *
 * @param cmd    파싱 결과 t_cmd 연결 리스트
 * @param shell  셸 상태 (exit_status 업데이트됨)
 * @return       마지막 명령어의 종료 상태
 */
int	execute(t_cmd *cmd, t_data *shell)
{
	int		**pipes;
	int		cmd_count;
	int		i;
	int		status;
	pid_t		*pids;
	t_cmd		*current;
	char		*cmd_path;
	char		buf[256];

	if (!cmd || !cmd->argv)
		return (-1);

	// 단일 명령어: 파이프 없이 처리
	if (!cmd->next)
		return (execute_single(shell, cmd));

	// 파이프라인 처리
	cmd_count = count_cmd(cmd);

	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	if (!pipes)
	{
		perror("minishell: malloc");
		return (-1);
	}
	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("minishell: pipe");
			free(pipes);
			return (-1);
		}
		i++;
	}

	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		perror("minishell: malloc");
		return (-1);
	}

	current = cmd;
	i = 0;
	while (current)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("minishell: fork");
			close_pipes(pipes, cmd_count - 1);
			free(pipes);
			free(pids);
			return (-1);
		}
		if (pids[i] == 0)
		{
			// stdin 연결: 첫 번째 명령어가 아니면 이전 파이프 read 끝
			if (i > 0)
				dup2(pipes[i - 1][0], STDIN_FILENO);
			// stdout 연결: 마지막 명령어가 아니면 현재 파이프 write 끝
			if (i < cmd_count - 1)
				dup2(pipes[i][1], STDOUT_FILENO);
			close_pipes(pipes, cmd_count - 1);
			// 리다이렉션 적용
			if (apply_redir(current->redir, shell) == -1)
				exit(1);
			// 빌트인은 파이프 안에서도 fork된 자식에서 실행
			if (is_builtin(current->argv[0]))
				exit(execute_builtin(shell, current->argv));
			cmd_path = find_command_path(current->argv[0], shell->envp);
			if (!cmd_path)
			{
				write(2, "minishell: ", 11);
				write(2, current->argv[0], ft_strlen(current->argv[0]));
				write(2, ": command not found\n", 20);
				exit(127);
			}
			execve(cmd_path, current->argv, shell->envp);
			// execve 실패
			ft_strlcpy(buf, "minishell: ", sizeof(buf));
			ft_strlcat(buf, current->argv[0], sizeof(buf));
			perror(buf);
			exit(126);
		}
		current = current->next;
		i++;
	}

	// 부모: 모든 파이프 닫기
	close_pipes(pipes, cmd_count - 1);

	// 모든 자식 대기, 마지막 명령어의 exit_status 저장
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->exit_status = 128 + WTERMSIG(status);
		}
		i++;
	}

	// 메모리 해제
	i = 0;
	while (i < cmd_count - 1)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
	free(pids);
	return (shell->exit_status);
}
