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

static int	check_syntax(t_token *tok, t_data *data)
{
	t_tok_type	prev;

	if (!tok)
		return (0);
	if (tok->type == TOK_PIPE)
		return (syntax_err(data, "|"));
	prev = TOK_WORD;
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

static t_token	*fill_segment(t_cmd *cur, t_token *tok)
{
	int	idx;

	idx = 0;
	while (tok && tok->type != TOK_PIPE)
	{
		if (tok->type == TOK_WORD)
		{
			if (!cur->cmd)
				cur->cmd = ft_strdup(tok->str);
			else
				cur->argv[idx++] = ft_strdup(tok->str);
		}
		else
		{
			redir_from_token(cur, tok);
			tok = tok->next;
		}
		tok = tok->next;
	}
	return (tok);
}

static t_cmd	*build_cmds(t_token *tok)
{
	t_cmd	*head;
	t_cmd	*prev;
	t_cmd	*cur;

	head = NULL;
	prev = NULL;
	while (tok)
	{
		cur = new_cmd();
		if (!cur)
			return (free_cmd_list(head), NULL);
		if (!alloc_argv(cur, tok))
			return (free_cmd_list(head), NULL);
		if (!head)
			head = cur;
		else
			prev->next = cur;
		prev = cur;
		tok = fill_segment(cur, tok);
		if (tok)
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
