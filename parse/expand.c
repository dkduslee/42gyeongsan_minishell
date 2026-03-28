/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 00:00:00 by seoyeoki          #+#    #+#             */
/*   Updated: 2026/03/26 19:40:52 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_env	*find_env_exact(t_env *head, const char *key)
{
	while (head)
	{
		if (ft_strncmp(head->key, key, ft_strlen(head->key) + 1) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

char	*str_append(char *s, char *add)
{
	char	*res;

	if (!s)
		return (ft_strdup(add));
	res = ft_strjoin(s, add);
	free(s);
	return (res);
}

char	*expand_tilde(t_data *data)
{
	t_env	*node;

	node = find_env_exact(data->env, "HOME");
	if (!node || !node->value)
		return (ft_strdup("~"));
	return (ft_strdup(node->value));
}

char	*expand_dollar(char *input, int *i, t_data *data)
{
	char	*var;
	t_env	*node;
	int		start;

	(*i)++;
	if (input[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(data->exit_status));
	}
	start = *i;
	if (ft_isalpha(input[*i]) || input[*i] == '_')
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
			(*i)++;
	if (*i == start)
		return (ft_strdup("$"));
	var = ft_substr(input, start, *i - start);
	node = find_env_exact(data->env, var);
	free(var);
	if (!node || !node->value)
		return (ft_strdup(""));
	return (ft_strdup(node->value));
}

char	*expand_line(char *line, t_data *data)
{
	char	*res;
	char	*exp;
	int		i;
	int		start;

	res = NULL;
	i = 0;
	while (line[i])
	{
		start = i;
		while (line[i] && line[i] != '$')
			i++;
		if (i > start)
			exp = ft_substr(line, start, i - start);
		else
			exp = expand_dollar(line, &i, data);
		res = str_append(res, exp);
		free(exp);
	}
	if (!res)
		return (ft_strdup(""));
	return (res);
}
