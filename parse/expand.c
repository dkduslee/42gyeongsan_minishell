/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoyeoki <seoyeoki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 00:00:00 by seoyeoki          #+#    #+#             */
/*   Updated: 2026/03/03 00:00:00 by seoyeoki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../aylee/minishell.h"

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

char	*str_append_char(char *s, char c)
{
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	return (str_append(s, buf));
}

char	*expand_dollar(char *input, int *i, t_data *data)
{
	char	var[256];
	int		j;
	t_env	*node;

	(*i)++;
	if (input[*i] == '?')
		return ((*i)++, ft_itoa(data->exit_status));
	j = 0;
	if (ft_isalpha(input[*i]) || input[*i] == '_')
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_')
			&& j < 255)
			var[j++] = input[(*i)++];
	var[j] = '\0';
	if (j == 0)
		return (ft_strdup("$"));
	node = find_env_exact(data->env, var);
	if (!node || !node->value)
		return (ft_strdup(""));
	return (ft_strdup(node->value));
}

char	*expand_heredoc_line(char *line, t_data *data)
{
	char	*res;
	char	*exp;
	int		i;

	res = NULL;
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			exp = expand_dollar(line, &i, data);
			res = str_append(res, exp);
			free(exp);
		}
		else
			res = str_append_char(res, line[i++]);
	}
	if (!res)
		return (ft_strdup(""));
	return (res);
}
