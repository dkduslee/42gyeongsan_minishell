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

static void	lex_unsupported(char *input, int *i, t_lex *lx)
{
	char	tok[3];
	int		j;
	char	c;
	char	next;

	c = input[*i];
	next = input[*i + 1];
	j = 0;
	tok[j++] = c;
	if ((c == '&' && next == '&')
		|| (c == ';' && next == ';')
		|| (c == '|' && next == '|'))
		tok[j++] = next;
	tok[j] = '\0';
	lx->err_token = ft_strdup(tok);
	lx->error = 2;
}

static void	lex_op(char *input, int *i, t_lex *lx)
{
	t_token	*tok;

	tok = NULL;
	if (input[*i] == '|')
		tok = new_token(TOK_PIPE, ft_strdup("|"));
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		tok = new_token(TOK_HEREDOC, ft_strdup("<<"));
		(*i)++;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		tok = new_token(TOK_REDIR_APPEND, ft_strdup(">>"));
		(*i)++;
	}
	else if (input[*i] == '<')
		tok = new_token(TOK_REDIR_IN, ft_strdup("<"));
	else if (input[*i] == '>')
		tok = new_token(TOK_REDIR_OUT, ft_strdup(">"));
	(*i)++;
	append_token(lx, tok);
}

static void	lex_char(char *input, int *i, t_lex *lx, t_data *data)
{
	if (input[*i] == '\'')
		lex_single_quote(input, i, lx);
	else if (input[*i] == '"')
		lex_double_quote(input, i, lx, data);
	else if (input[*i] == '$' || (input[*i] == '~' && !lx->buf
			&& (input[*i + 1] == '/' || input[*i + 1] == '\0')))
		lex_char_expand(input, i, lx, data);
	else
		lex_char_plain(input, i, lx);
}

static int	lex_switch(char *input, int *i, t_lex *lx, t_data *data)
{
	char	c;
	char	next;

	c = input[*i];
	next = input[*i + 1];
	if (c == ' ' || c == '\t')
	{
		flush_word(lx);
		(*i)++;
	}
	else if (ft_strchr("&;()*\\", c) || (c == '|' && next == '|'))
	{
		flush_word(lx);
		lex_unsupported(input, i, lx);
		return (1);
	}
	else if (c == '|' || c == '<' || c == '>')
	{
		flush_word(lx);
		lex_op(input, i, lx);
	}
	else
		lex_char(input, i, lx, data);
	return (0);
}

t_token	*lexer(char *input, t_data *data)
{
	t_lex	lx;
	int		i;

	init_lex(&lx);
	i = 0;
	while (input[i])
		if (lex_switch(input, &i, &lx, data))
			break ;
	flush_word(&lx);
	if (lx.error)
	{
		free_tokens(lx.head);
		if (lx.error == 1)
		{
			err_unclosed_quote();
			data->exit_status = 1;
		}
		else if (lx.error == 2)
		{
			err_syntax_token(lx.err_token);
			free(lx.err_token);
			data->exit_status = 2;
		}
		return (NULL);
	}
	return (lx.head);
}
