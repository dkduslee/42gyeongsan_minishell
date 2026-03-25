/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoyeoki <seoyeoki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 00:00:00 by seoyeoki          #+#    #+#             */
/*   Updated: 2026/03/25 00:00:00 by seoyeoki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

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

void	lex_char_expand(char *input, int *i, t_lex *lx, t_data *data)
{
	char	*exp;

	if (input[*i] == '$')
		exp = expand_dollar(input, i, data);
	else
	{
		exp = expand_tilde(data);
		(*i)++;
	}
	lx->buf = str_append(lx->buf, exp);
	free(exp);
}

void	lex_char_plain(char *input, int *i, t_lex *lx)
{
	char	*chunk;
	int		start;

	start = *i;
	while (input[*i] && !ft_strchr(" \t|<>&;()*$'\"", input[*i]))
		(*i)++;
	chunk = ft_substr(input, start, *i - start);
	lx->buf = str_append(lx->buf, chunk);
	free(chunk);
}
