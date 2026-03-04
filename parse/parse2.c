/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoyeoki <seoyeoki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 00:00:00 by seoyeoki          #+#    #+#             */
/*   Updated: 2026/03/03 00:00:00 by seoyeoki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

static int	syntax_err(t_data *data, char *msg)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("'\n", 2);
	data->exit_status = 2;
	return (-1);
}

static int	check_syntax(t_token *tok, t_data *data)
{
	t_tok_type	prev;

	if (!tok)
		return (0);
	prev = TOK_PIPE;
	while (tok)
	{
		if (tok->type == TOK_PIPE && prev == TOK_PIPE)
			return (syntax_err(data, "|"));
		if (tok->type != TOK_WORD && tok->type != TOK_PIPE)
		{
			if (!tok->next || tok->next->type != TOK_WORD)
				return (syntax_err(data, "newline"));
			if (prev == TOK_PIPE)
				return (syntax_err(data, tok->str));
		}
		prev = tok->type;
		tok = tok->next;
	}
	if (prev == TOK_PIPE)
		return (syntax_err(data, "|"));
	return (1);
}

static t_token	*add_redir(t_cmd *cmd, t_token *tok)
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
	return (file->next);
}

static t_cmd	*build_cmds(t_token *tok)
{
	t_cmd	*head;
	t_cmd	*cur;

	head = new_cmd();
	if (!head)
		return (NULL);
	cur = head;
	while (tok)
	{
		if (tok->type == TOK_PIPE)
		{
			cur->next = new_cmd();
			cur = cur->next;
		}
		else if (tok->type == TOK_WORD)
			add_argv(cur, tok->str);
		else
		{
			tok = add_redir(cur, tok);
			continue ;
		}
		tok = tok->next;
	}
	return (head);
}

t_cmd	*parse_pipeline(char *input, t_data *data)
{
	t_token	*tok;
	t_cmd	*cmd;
	int		chk;

	if (!input || !*input)
		return (NULL);
	tok = lexer(input, data);
	if (!tok)
		return (NULL);
	chk = check_syntax(tok, data);
	if (chk <= 0)
	{
		free_tokens(tok);
		return (NULL);
	}
	cmd = build_cmds(tok);
	free_tokens(tok);
	if (cmd && !cmd->cmd && !cmd->redir)
	{
		free_cmd_list(cmd);
		return (NULL);
	}
	return (cmd);
}
