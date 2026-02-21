/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 00:00:00 by aylee             #+#    #+#             */
/*   Updated: 2026/02/21 18:19:32 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_builtin(char *cmd)
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
	char	**args;
	int		ret;

	if (!input || !*input) //원래 스플릿 확인후에 수정
		return ;
	
	// 공백으로 split
	args = ft_split(input, ' ');
	if (!args || !args[0])
	{
		free_split(args);
		return ;
	}
	
	// builtin 명령어인지 확인
	if (is_builtin(args[0]))
	{
		ret = execute_builtin(data, args);
		data->exit_status = ret;
	}
	else
	{
		// printf("minishell: %s: command not found\n", args[0]);
		ret = execute_command(data, args);
		data->exit_status = ret;
	}
	
	free_split(args);
}

void	print_error(t_data *data, const char *msg) //fprintf을 안 쓰고 해야 함
{
	printf("Error: %s\n", msg);
	data->exit_status = 1;
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
		print_error(data, "Failed to initialize data"); //이거 상태 이상하긴 한데 나중에 함수 만들어서 수정.
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

