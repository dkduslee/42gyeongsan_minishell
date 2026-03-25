/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 19:03:50 by aylee             #+#    #+#             */
/*   Updated: 2026/03/02 19:05:05 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_env_node(const char *key, const char *value)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (value != NULL)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	free_env_node(t_env *node)
{
	if (node)
	{
		free(node->key);
		if (node->value)
			free(node->value);
		free(node);
	}
}

t_env	*find_env_node(t_env *head, const char *key)
{
	t_env	*current;

	current = head;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

t_env	*add_env_node(t_env **head, const char *key, const char *value)
{
	t_env	*new_node;
	t_env	*current;

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

void	delete_env(t_env **head, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *head;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*head = current->next;
			free_env_node(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
