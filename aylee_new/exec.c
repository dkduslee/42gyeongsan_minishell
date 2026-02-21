/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:00:00 by aylee             #+#    #+#             */
/*   Updated: 2026/02/21 17:10:28 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * env 연결 리스트에서 PATH 값을 꺼내
 * cmd를 실행 가능한 전체 경로로 찾아 반환한다.
 *
 * 반환값: 찾은 경로 (malloc, 호출자가 free 해야 함)
 *         못 찾으면 NULL
 */
char	*find_command_path(const char *cmd, t_env *env)
{
	t_env	*path_node;
	char	**dirs;
	char	*full_path;
	char	*tmp;
	int		i;

	// cmd에 '/'가 포함되면 경로 직접 지정으로 판단
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_node = find_env_node(env, "PATH");
	if (!path_node || !path_node->value)
		return (NULL);

	// PATH 값을 ':'로 분리
	dirs = ft_split(path_node->value, ':');
	if (!dirs)
		return (NULL);

	i = 0;
	full_path = NULL;
	while (dirs[i])
	{
		// dir + "/" + cmd 조합
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

/*
 * 외부 명령어 실행 (ls, cat 등)
 * fork → execve 방식으로 실행하고 exit_status 업데이트
 *
 * @return  명령어 종료 코드
 */
int	execute_command(t_data *data, char **args)
{
	char	*cmd_path;
	char	**envp;
	pid_t	pid;
	int		status;
	char	buf[256];

	cmd_path = find_command_path(args[0], data->env);
	if (!cmd_path)
	{
		write(2, "minishell: ", 11);
		write(2, args[0], ft_strlen(args[0]));
		write(2, ": command not found\n", 20);
		data->exit_status = 127;
		return (127);
	}

	// t_env 리스트를 execve용 char** 환경변수 배열로 변환
	envp = env_to_array(data->env);

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		free(cmd_path);
		free_split(envp);
		return (-1);
	}
	if (pid == 0)
	{
		execve(cmd_path, args, envp);
		// execve 실패 시
		ft_strlcpy(buf, "minishell: ", sizeof(buf));
		ft_strlcat(buf, args[0], sizeof(buf));
		perror(buf);
		exit(126);
	}
	// 부모: 자식 대기
	waitpid(pid, &status, 0);
	free(cmd_path);
	free_split(envp);
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->exit_status = 128 + WTERMSIG(status);
	return (data->exit_status);
}
