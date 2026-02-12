/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 14:46:53 by aylee             #+#    #+#             */
/*   Updated: 2026/02/12 13:21:51 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env *create_env_node(const char *key, const char *value) //환경변수의 노드를 생성.
{
	t_env *new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return NULL;
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	return new_node;
}

void free_env_node(t_env *node)
{
	if (node)
	{
		free(node->key);
		free(node->value);
		free(node);
	}
}

t_env *find_env_node(t_env *head, const char *key) //환경변수의 노드를 찾음.
{
	t_env *current = head;

	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

t_env *add_env_node(t_env **head, const char *key, const char *value) //환경변수의 노드를 추가.
{
	t_env *new_node;
	t_env *current;

	new_node = create_env_node(key, value); //value가 null일 때도 있음.
	if (!new_node)
		return (NULL);
	if (*head == NULL)
	{
		*head = new_node;
		return (new_node);
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
	return (new_node);
}

t_env *parse_env(char **envp) //env 파싱 함수
{
	t_env 	*env_list;
	size_t	 key_len;
	int 	i;
	char	*key;
	char	*value;
	char	*equal_sign;

	i = 0;
	env_list = NULL;
	while (envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign) //찾았으면?
		{
			key_len = equal_sign - envp[i];
			key = (char *)malloc(key_len + 1);
			ft_strncpy(key, envp[i], key_len);
			key[key_len] = '\0';
			value = ft_strdup(equal_sign + 1);
			add_env_node(&env_list, key, value);
			free(key);
			free(value);
		}
		i++;
	}
	return (env_list);
}

void free_env_list(t_env *head) //환경변수 리스트 해제 함수
{
	t_env *current = head;
	t_env *next_node;

	while (current)
	{
		next_node = current->next;
		free_env_node(current);
		current = next_node;
	}
}

void delete_env(t_env *head, char *key) //환경변수 삭제 함수
{
	t_env *current = head;
	t_env *prev = NULL;

	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				head = current->next;
			free_env_node(current);
			current = (prev) ? prev->next : head;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

void print_env_list(t_env *head) //환경변수 리스트 출력 함수
{
	t_env *current = head;

	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

t_data	*init_data(char **envp)
{
	t_data	*data;

	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->env = parse_env(envp);
	data->exit_status = 0;
	return (data);
}

// int main(int argc, char **argv, char **envp) //이게 지금 envp 파싱끝.
// {
// 	t_env *env_list;

// 	(void)argc;
// 	(void)argv;

// 	env_list = parse_env(envp);
// 	print_env_list(env_list);
// 	free_env_list(env_list);
// 	return 0;
// }
