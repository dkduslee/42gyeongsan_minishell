/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 14:46:49 by aylee             #+#    #+#             */
/*   Updated: 2026/02/24 18:05:19 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// echo 구현
int	builtin_echo(t_data *data, char **args)
{
	int i;
	int newline;
	
	newline = 1;
	i = 1;
	if (args[1] && ft_strncmp(args[1], "-n", 3) == 0) // -nnnn 이런거 처리 (파싱부)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		if (ft_strncmp(args[i], "$?", 3) == 0)
			printf("%d", data->exit_status);
		else
			printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return 0;
}

// cd 구현
int	builtin_cd(t_data *data, char **args) //절대경로랑 상대경로만 있는 경우 구현하기.
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
			printf("Minishell: cd: HOME not set\n");
			data->exit_status = 1;
			return 1;
		}
	}
	else
	{
		path = args[1];
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return 1;
	}
	return 0;
}

// pwd 구현
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

static int	is_valid_identifier(const char *str)
{
	if (!str || !*str)
		return 0;
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return 0;
	while (*str && *str != '=')
	{
		if (!ft_isalnum(*str) && *str != '_')
			return 0;
		str++;
	}
	return (1);
}

// export 구현 (KEY=VALUE 형태 파싱)
int	builtin_export(t_data *data, char **args)

{
	char	*equal_sign;
	char	*key;
	char	*value;
	size_t	key_len;
	int		i;
	t_env	*current;

	// 인자가 없으면 모든 환경 변수 출력 (export 형태로)
	if (args[1] == NULL)
	{
		current = data->env;
		while (current)
		{
			if (current->value)
				printf("declare -x %s=\"%s\"\n", current->key, current->value);
			else
				printf("declare -x %s\n", current->key);
			current = current->next;
		}
		return 0;
	}
	
	// 각 인자에 대해 처리
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			printf("minishell: export: `%s': not a valid identifier\n", args[i]);
			i++;
			continue;
		}
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign) // KEY=VALUE 형태
		{
			key_len = equal_sign - args[i];
			key = (char *)malloc(key_len + 1);
			ft_strlcpy(key, args[i], key_len + 1);
			value = equal_sign + 1;
			set_env_var(data, key, value);
			free(key);
		}
		else // KEY만 있는 경우 (value는 NULL)
		{
			if (!find_env_node(data->env, args[i]))
				add_env_node(&data->env, args[i], NULL);
		}
		i++;
	}
	return 0;
}

int	set_env_var(t_data *data, const char *key, const char *value)
{
	t_env *node;

	node = find_env_node(data->env, key);
	if (node)
	{
		if (node->value)
			free(node->value);
		node->value = ft_strdup(value);
	}
	else
	{
		add_env_node(&data->env, key, value);
	}
	return 0;
}

// unset 구현
int	builtin_unset(t_data *data, char **args)
{
	int	i;

	if (args[1] == NULL)
		return 0;
	
	i = 1;
	while (args[i])
	{
		delete_env(&data->env, args[i]);
		i++;
	}
	return 0;
}

// exit 구현
int	builtin_exit(t_data *data, char **args)
{
	int status = 0;
	
	printf("exit\n");
	if (args[1]) //개수가 여러개로 들어와도 오류처리.
		status = ft_atoi(args[1]); // 큰 수가 오면 255로 exit하고, 문자가 오면 2로 exit하고 오류구문 내보냄.
	if (args[2])
	{
		status = 1;
		print_error_msg(data, args[0], "too many arguments", 1);
	}
	clean_up(data);
	exit(status);
}

// env 구현
int	builtin_env(t_data *data)
{
	print_env_list(data->env); // value가 있는 것만 출력
	return 0;
}

// 실행함수
int	execute_builtin(t_data *data, char **args)
{
	if (ft_strncmp(args[0], "echo", 5) == 0)
		return builtin_echo(data, args);
	else if (ft_strncmp(args[0], "cd", 3) == 0)
		return builtin_cd(data, args);
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
		return builtin_pwd();
	else if (ft_strncmp(args[0], "export", 7) == 0)
		return builtin_export(data, args);
	else if (ft_strncmp(args[0], "unset", 6) == 0)
		return builtin_unset(data, args);
	else if (ft_strncmp(args[0], "exit", 5) == 0)
		return builtin_exit(data, args);
	else if (ft_strncmp(args[0], "env", 4) == 0)
		return builtin_env(data);
	return -1;
}

