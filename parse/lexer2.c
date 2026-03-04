/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoyeoki <seoyeoki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 00:00:00 by seoyeoki          #+#    #+#             */
/*   Updated: 2026/03/03 00:00:00 by seoyeoki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

static void	lex_op(char *in, int *i, t_lex *lx)
{
	t_token	*tok;

	tok = NULL;
	if (in[*i] == '|')
		tok = new_token(TOK_PIPE, ft_strdup("|"));
	else if (in[*i] == '<' && in[*i + 1] == '<')
	{
		tok = new_token(TOK_HEREDOC, ft_strdup("<<"));
		(*i)++;
	}
	else if (in[*i] == '>' && in[*i + 1] == '>')
	{
		tok = new_token(TOK_REDIR_APPEND, ft_strdup(">>"));
		(*i)++;
	}
	else if (in[*i] == '<')
		tok = new_token(TOK_REDIR_IN, ft_strdup("<"));
	else
		tok = new_token(TOK_REDIR_OUT, ft_strdup(">"));
	(*i)++;
	if (!lx->head)
		lx->head = tok;
	else
		lx->tail->next = tok;
	lx->tail = tok;
}

static void	lex_char(char *in, int *i, t_lex *lx, t_data *data)
{
	char	*exp;

	if (in[*i] == '\'')
		lex_sq(in, i, lx);
	else if (in[*i] == '"')
		lex_dq(in, i, lx, data);
	else if (in[*i] == '$')
	{
		exp = expand_dollar(in, i, data);
		lx->buf = str_append(lx->buf, exp);
		free(exp);
	}
	else
	{
		lx->buf = str_append_char(lx->buf, in[*i]);
		(*i)++;
	}
}

t_token	*lexer(char *input, t_data *data)
{
	t_lex	lx;
	int		i;

	lx.head = NULL;
	lx.tail = NULL;
	lx.buf = NULL;
	lx.quoted = 0;
	lx.error = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == ' ' || input[i] == '\t')
		{
			flush_word(&lx);
			i++;
		}
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			flush_word(&lx);
			lex_op(input, &i, &lx);
		}
		else
			lex_char(input, &i, &lx, data);
	}
	flush_word(&lx);
	if (lx.error)
	{
		free_tokens(lx.head);
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		data->exit_status = 2;
		return (NULL);
	}
	return (lx.head);
}
