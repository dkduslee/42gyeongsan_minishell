/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 14:46:49 by aylee             #+#    #+#             */
/*   Updated: 2026/02/12 13:25:01 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//여기서 char **args는 기본적으로 넣어둔 배열. 파싱부의 형태보고 고치기.
//echo 구현
int	builtin_echo(char **args)
{
	int i;
	int newline;
	
	newline = 1;
	i = 1;
	if (args[1] && ft_strcmp(args[1], "-n") == 0)
	{
		newline = 0; //이 옵션이 있음 뉴라인안함.
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return 0;
}

//cd 구현
int	builtin_cd(t_data *data, char **args) //data에 env
{
	const char 	*path;
	t_env 		*home_node;

	if (args[1] == NULL)
	{
		home_node = find_env_node(data->env, "HOME");
		if (home_node)
			path = home_node->value;
		else
		{
			fprintf(stderr, "cd: HOME not set\n"); // 이거 써도 되나?
			return 1;
		}
	}
	else
	{
		path = args[1];
	}
	if (chdir(path) != 0) //실패
	{
		perror("cd");
		return 1;
	}
	return 0;
}

//pwd 구현
int	builtin_pwd(void)
{
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return 0;
	}
	perror("pwd");
	return 1;
}

//export 구현
int	builtin_export(t_data *data, char **args)
{
	if (args[1] == NULL)
	{
		print_env(data->env);
		return 0;
	}
	return set_env_var(data, args[1], args[2]); //이거 지금 함수가 없음. env에 추가하는 함수
}

int	set_env_var(t_data *data, const char *key, const char *value)
{
	t_env *node;

	node = find_env_node(data->env, key);
	if (node)
	{
		free(node->value);
		node->value = strdup(value);
	}
	else
	{
		add_env_node(&data->env, key, value);
	}
	return 0;
} //복사 env에 넣는 구조인데? env라고만 치면 value가 없는 것은 출력을 안 해야함.
//=으로 구분이 되어있는지. 아!!!! value가 없는 걸 기준으로 하기.

//unset 구현
int	builtin_unset(t_data *data, char **args)
{
	if (args[1] == NULL)
	{
		// print_error("unset", "not enough arguments", 0); 아무것도 안 떠야 함.
		return 0;
	}
	return (delete_env(data->env, args[1]), 0);
}

//exit 구현
int	builtin_exit(t_data *data, char **args)
{
	int status = 0;
	if (args[1])
		status = ft_atoi(args[1]); //bash에서는 long으로 되어있음. 그냥 쓰면 이상하다고 함.
	clean_up(data);
	exit(status);
}

//env 구현
int	builtin_env(t_data *data)
{
	if (data->env->value != NULL) //이런식으로 value가 있는 것만 출력. 추후 수정.
		print_env(data->env);
	return 0;
}

//실행함수
int	execute_builtin(t_data *data, char **args)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return builtin_echo(args);
	else if (ft_strcmp(args[0], "cd") == 0)
		return builtin_cd(data, args);
	else if (ft_strcmp(args[0], "pwd") == 0)
		return builtin_pwd();
	else if (ft_strcmp(args[0], "export") == 0)
		return builtin_export(data, args);
	else if (ft_strcmp(args[0], "unset") == 0)
		return builtin_unset(data, args);
	else if (ft_strcmp(args[0], "exit") == 0)
		return builtin_exit(data, args);
	else if (ft_strcmp(args[0], "env") == 0)
		return builtin_env(data);
	return -1;
}

//테스트용 메인
int	main(int argc, char **argv, char **envp)
{
	t_data *data;

	// 초기화 및 환경 변수 설정
	data = init_data(envp);
	set_env_var(&data, "HOME", "/Users/aylee");

	if (argc > 1)
	{
		execute_builtin(&data, argv + 1);
	}
	clean_up(&data);
	return 0;
}