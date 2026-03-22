/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 00:00:00 by aylee             #+#    #+#             */
/*   Updated: 2026/03/02 16:51:13 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	return (0);
}

static void process_input(t_data *data, char *input)
{
	t_cmd *cmd;

	if (!input || !*input)
		return;
	cmd = parse_pipeline(input, data);
	if (!cmd)
		return;
	g_signal = 0;
	execute_pipeline(data, cmd);
	if (g_signal == SIGINT)
		data->exit_status = 130;
	g_signal = 0;
	free_cmd_list(cmd);
}

void print_error(t_data *data, char *cmd, int err_num, int exit_code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(err_num), 2);
	data->exit_status = exit_code;
}

void print_error_msg(t_data *data, char *cmd, char *msg, int exit_code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	data->exit_status = exit_code;
}

int main(int argc, char **argv, char **envp) // 이후에 수정.
{
	t_data data;
	char *line;

	(void)argc;
	(void)argv;
	init_data(&data, envp);
	if (!data.env)
	{
		ft_putendl_fd("minishell: failed to initialize", 2);
		return (1);
	}
	shell_init();
	signal_interactive();
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			ft_putendl_fd("exit", 1);
			break;
		}
		if (*line)
		{
			add_history(line);
			process_input(&data, line);
		}
		else if (g_signal == SIGINT)
			data.exit_status = 130;
		g_signal = 0;
		signal_interactive();
		free(line);
		line = NULL;
	}
	clean_up(&data);
	return (data.exit_status);
}
