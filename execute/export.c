/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 18:58:18 by aylee             #+#    #+#             */
/*   Updated: 2026/03/02 19:00:41 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(const char *str)
{
	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	while (*str && *str != '=')
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

void	default_export(t_data *data)
{
	t_env	*current;

	current = data->env;
	while (current)
	{
		if (current->value)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
}

void	make_env_new(t_data *data, char **args, char *equal_sign, int i)
{
	int		key_len;
	char	*key;
	char	*value;

	key_len = equal_sign - args[i];
	key = (char *)malloc(key_len + 1);
	if (!key)
		return ;
	ft_strlcpy(key, args[i], key_len + 1);
	value = equal_sign + 1;
	set_env_var(data, key, value);
	free(key);
}

int	set_env_var(t_data *data, const char *key, const char *value)
{
	t_env	*node;

	node = find_env_node(data->env, key);
	if (node)
	{
		if (node->value)
			free(node->value);
		node->value = ft_strdup(value);
	}
	else
		add_env_node(&data->env, key, value);
	return (0);
}

int	builtin_export(t_data *data, char **args)
{
	char	*equal_sign;
	int		i;

	i = 0;
	if (!args || args[0] == NULL)
		return (default_export(data), 0);
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			printf("minishell: export: `%s'", args[i++]);
			printf(": not a valid identifier\n");
			data->exit_status = 1;
			continue ;
		}
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
			make_env_new(data, args, equal_sign, i);
		else if (!find_env_node(data->env, args[i]))
			add_env_node(&data->env, args[i], NULL);
		i++;
	}
	return (0);
}
