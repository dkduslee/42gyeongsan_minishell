/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoyeoki <seoyeoki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 00:00:00 by seoyeoki          #+#    #+#             */
/*   Updated: 2026/03/03 00:00:00 by seoyeoki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->cmd = NULL;
	cmd->argv = NULL;
	cmd->redir = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	free_redir_list(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->file);
		free(redir);
		redir = next;
	}
}

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*next;
	int		i;

	while (cmd)
	{
		next = cmd->next;
		free(cmd->cmd);
		if (cmd->argv)
		{
			i = 0;
			while (cmd->argv[i])
				free(cmd->argv[i++]);
			free(cmd->argv);
		}
		free_redir_list(cmd->redir);
		free(cmd);
		cmd = next;
	}
}

void	add_argv(t_cmd *cmd, char *word)
{
	int		count;
	char	**new_argv;
	int		i;

	if (!cmd->cmd)
	{
		cmd->cmd = ft_strdup(word);
		return ;
	}
	count = 0;
	while (cmd->argv && cmd->argv[count])
		count++;
	new_argv = malloc(sizeof(char *) * (count + 2));
	if (!new_argv)
		return ;
	i = 0;
	while (i < count)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[count] = ft_strdup(word);
	new_argv[count + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
}

void	redir_append(t_cmd *cmd, t_redir_type type, char *file, int quoted)
{
	t_redir	*node;
	t_redir	*cur;

	node = malloc(sizeof(t_redir));
	if (!node)
		return ;
	node->type = type;
	node->file = ft_strdup(file);
	node->fd = -1;
	node->quoted = quoted;
	node->next = NULL;
	if (!cmd->redir)
	{
		cmd->redir = node;
		return ;
	}
	cur = cmd->redir;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}
