/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 00:00:00 by aylee             #+#    #+#             */
/*   Updated: 2026/02/23 14:56:35 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
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

static void	process_input(t_data *data, char *input)
{
	t_cmd	*cmd;

	if (!input || !*input)
		return ;
	cmd = parse_pipeline(input);
	if (!cmd)
		return ;
	execute_pipeline(data, cmd);
	free_cmd_list(cmd);
}

void	print_error(t_data *data, char *cmd, int err_num, int exit_code)
{
	//strerror (errno)로 처리
	printf("Minishell: %s : %s\n", cmd, strerror(err_num));
	data->exit_status = exit_code;
}

void	print_error_msg(t_data *data, char *cmd, char *msg, int exit_code)
{
	printf("minishell: %s: %s\n", cmd, msg);
	data->exit_status = exit_code;
}

int	main(int argc, char **argv, char **envp) //방향키가 안 먹는 중.
{
	t_data	*data;
	char	*line;

	(void)argc;
	(void)argv;
	
	// 데이터 초기화
	data = init_data(envp);
	if (!data)
	{
		print_error(data, "Failed to initialize data", errno, 1); //이거 상태 이상하긴 한데 나중에 함수 만들어서 수정.
		return (1);
	}
	
	// 입력 루프
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break;
		}
		if (*line)
		{
			add_history(line);
			//파싱? (data);
			process_input(data, line);
		}
		free(line);
		line = NULL;
	}
	// 정리
	clean_up(data);
	return (data->exit_status);
}

