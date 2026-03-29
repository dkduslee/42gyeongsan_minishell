/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 13:27:25 by aylee             #+#    #+#             */
/*   Updated: 2026/03/26 19:08:08 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_exit_status(t_data *data, int status)
{
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		data->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", 2);
	}
}

int	wait_child(t_data *data, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	update_exit_status(data, status);
	return (data->exit_status);
}

static void	exec_after_fork(char *path, char **args, char **envp)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	execve(path, args, envp);
	perror("minishell");
	free(path);
	exit(126);
}

int	fork_and_exec(t_data *data, t_cmd *cmd, char *cmd_path)
{
	char	**envp;
	char	**args;
	pid_t	pid;

	envp = env_to_array(data->env);
	args = get_execve_args(cmd);
	pid = fork();
	if (pid == -1)
	{
		print_error(data, "fork", errno, 1);
		free_split(envp);
		free_split(args);
		return (free(cmd_path), 1);
	}
	if (pid == 0)
	{
		rl_clear_history();
		free_cmd_list(cmd);
		free_env_list(data->env);
		exec_after_fork(cmd_path, args, envp);
	}
	free(cmd_path);
	free_split(envp);
	free_split(args);
	return (wait_child(data, pid));
}

int	execute_command(t_data *data, t_cmd *cmd)
{
	char	*cmd_path;

	if (!cmd || !cmd->cmd)
		return (0);
	if (is_builtin(cmd->cmd))
		return (execute_builtin(data, cmd));
	cmd_path = find_command_path(cmd->cmd, data->env);
	if (!cmd_path)
	{
		print_error_msg(data, cmd->cmd, "command not found", 127);
		return (127);
	}
	return (fork_and_exec(data, cmd, cmd_path));
}

int	execute_builtin(t_data *data, t_cmd *cmd)
{
	if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
		return (builtin_echo(data, cmd->argv));
	else if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
		return (builtin_cd(data, cmd->argv));
	else if (ft_strncmp(cmd->cmd, "pwd", 4) == 0)
		return (builtin_pwd(data));
	else if (ft_strncmp(cmd->cmd, "export", 7) == 0)
		return (builtin_export(data, cmd->argv));
	else if (ft_strncmp(cmd->cmd, "unset", 6) == 0)
		return (builtin_unset(data, cmd->argv));
	else if (ft_strncmp(cmd->cmd, "exit", 5) == 0)
		return (builtin_exit(data, cmd, cmd->argv));
	else if (ft_strncmp(cmd->cmd, "env", 4) == 0)
		return (builtin_env(data, cmd->argv));
	return (-1);
}
