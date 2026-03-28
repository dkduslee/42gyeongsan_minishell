/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoyeoki <seoyeoki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 00:00:00 by seoyeoki          #+#    #+#             */
/*   Updated: 2026/03/28 00:00:00 by seoyeoki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

int	count_words(t_token *tok)
{
	int	count;

	count = 0;
	while (tok && tok->type != TOK_PIPE)
	{
		if (tok->type == TOK_WORD)
			count++;
		else
			tok = tok->next;
		tok = tok->next;
	}
	return (count);
}

int	alloc_argv(t_cmd *cmd, t_token *tok)
{
	int	wc;

	wc = count_words(tok);
	if (wc <= 1)
		return (1);
	cmd->argv = malloc(sizeof(char *) * wc);
	if (!cmd->argv)
		return (0);
	cmd->argv[wc - 1] = NULL;
	return (1);
}
