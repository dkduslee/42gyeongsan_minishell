/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_int.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoyeoki <seoyeoki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 00:00:00 by seoyeoki          #+#    #+#             */
/*   Updated: 2026/03/03 00:00:00 by seoyeoki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_INT_H
# define PARSE_INT_H

# include "../aylee/minishell.h"

typedef struct s_lex
{
	t_token		*head;
	t_token		*tail;
	char		*buf;
	int			quoted;
	int			error;
}	t_lex;

/* lexer.c → lexer2.c */
void	flush_word(t_lex *lx);
void	lex_sq(char *in, int *i, t_lex *lx);
void	lex_dq(char *in, int *i, t_lex *lx, t_data *data);

/* parse.c → parse2.c */
void	add_argv(t_cmd *cmd, char *word);
void	redir_append(t_cmd *cmd, t_redir_type type, char *file, int quoted);

#endif
