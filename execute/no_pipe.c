/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 18:04:10 by aylee             #+#    #+#             */
/*   Updated: 2026/03/29 18:11:02 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_state_builtin(t_cmd *cmd)
{
	return (ft_strncmp(cmd->cmd, "exit", 5) == 0
		|| ft_strncmp(cmd->cmd, "cd", 3) == 0
		|| ft_strncmp(cmd->cmd, "export", 7) == 0
		|| ft_strncmp(cmd->cmd, "unset", 6) == 0);
}

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
		if (!cmd->redir || is_state_builtin(cmd))
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
