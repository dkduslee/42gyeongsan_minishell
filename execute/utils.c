/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 00:00:00 by aylee             #+#    #+#             */
/*   Updated: 2026/03/02 18:57:53 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void clean_up(t_data *data)
{
	if (data)
	{
		if (data->env)
			free_env_list(data->env);
	}
}

void free_split(char **split)
{
	int i;

	if (!split)
		return;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void merge_key_value(t_env *env, char **arr)
{
	t_env *cur;
	int i;
	char *tmp;

	cur = env;
	i = 0;
	while (cur)
	{
		if (cur->value)
		{
			tmp = ft_strjoin(cur->key, "=");
			arr[i] = ft_strjoin(tmp, cur->value);
			free(tmp);
		}
		else
			arr[i] = ft_strdup(cur->key);
		i++;
		cur = cur->next;
	}
	arr[i] = NULL;
}

char **env_to_array(t_env *env)
{
	t_env *cur;
	char **arr;
	int count;

	count = 0;
	cur = env;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	arr = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	merge_key_value(env, arr);
	return (arr);
}
