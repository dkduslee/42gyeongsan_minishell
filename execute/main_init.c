/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 12:53:41 by aylee             #+#    #+#             */
/*   Updated: 2026/03/26 19:37:44 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_data(t_data *data, char **envp)
{
	data->env = parse_env(envp);
	data->exit_status = 0;
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

void	free_pid(t_pipes *pipeline)
{
	if (pipeline->pipes)
		free(pipeline->pipes);
	if (pipeline->pids)
		free(pipeline->pids);
	pipeline->pipes = NULL;
	pipeline->pids = NULL;
}

void	init_pipes(t_data *data, t_cmd *cmd, t_pipes *pipeline)
{
	int	i;

	pipeline->count = count_cmd(cmd);
	pipeline->pids = malloc(sizeof(pid_t) * pipeline->count);
	pipeline->pipes = malloc(sizeof(int *) * (pipeline->count - 1));
	if (!pipeline->pipes || !pipeline->pids)
	{
		print_error(data, "malloc", errno, 1);
		free_pid(pipeline);
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
			free_pid(pipeline);
			return ;
		}
		i++;
	}
}
