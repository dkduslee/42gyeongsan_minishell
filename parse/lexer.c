/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoyeoki <seoyeoki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 00:00:00 by seoyeoki          #+#    #+#             */
/*   Updated: 2026/03/03 00:00:00 by seoyeoki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

t_token	*new_token(t_tok_type type, char *str)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->type = type;
	tok->str = str;
	tok->quoted = 0;
	tok->next = NULL;
	return (tok);
}

void	free_tokens(t_token *head)
{
	t_token	*next;

	while (head)
	{
		next = head->next;
		free(head->str);
		free(head);
		head = next;
	}
}

void	flush_word(t_lex *lx)
{
	t_token	*tok;

	if (!lx->buf)
		return ;
	tok = new_token(TOK_WORD, lx->buf);
	if (!tok)
		return ;
	tok->quoted = lx->quoted;
	lx->buf = NULL;
	lx->quoted = 0;
	if (!lx->head)
		lx->head = tok;
	else
		lx->tail->next = tok;
	lx->tail = tok;
}

void	lex_single_quote(char *input, int *i, t_lex *lx)
{
	char	*chunk;
	int		start;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	chunk = ft_substr(input, start, *i - start);
	lx->buf = str_append(lx->buf, chunk);
	free(chunk);
	if (input[*i] == '\'')
		(*i)++;
	else
		lx->error = 1;
	lx->quoted = 1;
}

void	lex_double_quote(char *input, int *i, t_lex *lx, t_data *data)
{
	char	*chunk;
	char	*exp;
	int		start;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '"')
		(*i)++;
	chunk = ft_substr(input, start, *i - start);
	exp = expand_line(chunk, data);
	free(chunk);
	lx->buf = str_append(lx->buf, exp);
	free(exp);
	if (input[*i] == '"')
		(*i)++;
	else
		lx->error = 1;
	lx->quoted = 1;
}
