/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 14:46:53 by aylee             #+#    #+#             */
/*   Updated: 2026/02/21 16:02:36 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env *create_env_node(const char *key, const char *value)
{
	t_env *new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return NULL;
	new_node->key = ft_strdup(key);
	new_node->value = value ? ft_strdup(value) : NULL; // value가 NULL일 수 있음
	new_node->next = NULL;
	return new_node;
}

void free_env_node(t_env *node)
{
	if (node)
	{
		free(node->key);
		if (node->value)
			free(node->value);
		free(node);
	}
}

t_env *find_env_node(t_env *head, const char *key)
{
	t_env *current = head;

	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

t_env *add_env_node(t_env **head, const char *key, const char *value)
{
	t_env *new_node;
	t_env *current;

	new_node = create_env_node(key, value);
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

t_env *parse_env(char **envp)
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
		if (equal_sign)
		{
			key_len = equal_sign - envp[i];
			key = (char *)malloc(key_len + 1);
			ft_strlcpy(key, envp[i], key_len + 1);
			value = ft_strdup(equal_sign + 1);
			add_env_node(&env_list, key, value);
			free(key);
			free(value);
		}
		i++;
	}
	return (env_list);
}

void free_env_list(t_env *head)
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

void delete_env(t_env **head, char *key) // head를 포인터의 포인터로 수정
{
	t_env *current = *head;
	t_env *prev = NULL;

	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*head = current->next; // head 업데이트
			free_env_node(current);
			return; // 찾았으면 종료
		}
		prev = current;
		current = current->next;
	}
}

void print_env_list(t_env *head)
{
	t_env *current = head;

	while (current)
	{
		if (current->value) // value가 있는 것만 출력 (env 명령어용)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

