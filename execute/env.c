/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 14:46:53 by aylee             #+#    #+#             */
/*   Updated: 2026/03/02 19:06:31 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sub_parse_env(char **envp, char *equal_sign, t_env **env_list, int i)
{
	char	*key;
	char	*value;
	int		key_len;

	key_len = equal_sign - envp[i];
	key = (char *)malloc(key_len + 1);
	if (!key)
		return (1);
	ft_strlcpy(key, envp[i], key_len + 1);
	value = ft_strdup(equal_sign + 1);
	add_env_node(env_list, key, value);
	free(key);
	free(value);
	return (0);
}

t_env	*parse_env(char **envp)
{
	t_env	*env_list;
	int		i;
	char	*equal_sign;

	i = 0;
	env_list = NULL;
	while (envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign)
		{
			if (sub_parse_env(envp, equal_sign, &env_list, i))
				return (env_list);
		}
		i++;
	}
	return (env_list);
}

void	free_env_list(t_env *head)
{
	t_env	*current;
	t_env	*next_node;

	current = head;
	while (current)
	{
		next_node = current->next;
		free_env_node(current);
		current = next_node;
	}
}

void	print_env_list(t_env *head)
{
	t_env	*current;

	current = head;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}
