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

void	redir_from_token(t_cmd *cmd, t_token *tok)
{
	t_redir_type	type;
	t_token			*file;

	file = tok->next;
	type = REDIR_IN;
	if (tok->type == TOK_REDIR_OUT)
		type = REDIR_OUT;
	else if (tok->type == TOK_REDIR_APPEND)
		type = REDIR_APPEND;
	else if (tok->type == TOK_HEREDOC)
		type = REDIR_HEREDOC;
	redir_append(cmd, type, file->str, file->quoted);
}
