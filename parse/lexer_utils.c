/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoyeoki <seoyeoki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 00:00:00 by seoyeoki          #+#    #+#             */
/*   Updated: 2026/03/25 00:00:00 by seoyeoki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

void	init_lex(t_lex *lx)
{
	lx->head = NULL;
	lx->tail = NULL;
	lx->buf = NULL;
	lx->quoted = 0;
	lx->error = 0;
	lx->err_token = NULL;
}

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

void	append_token(t_lex *lx, t_token *tok)
{
	if (!lx->head)
		lx->head = tok;
	else
		lx->tail->next = tok;
	lx->tail = tok;
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
	append_token(lx, tok);
}
