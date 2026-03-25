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

# include "../minishell.h"

typedef struct s_lex
{
	t_token		*head;
	t_token		*tail;
	char		*buf;
	int			quoted;
	int			error;
	char		*err_token;
}	t_lex;

/* error.c */
void	err_syntax_token(char *token);
void	err_unclosed_quote(void);

/* expand.c */
char	*expand_line(char *line, t_data *data);

/* lex_utils.c */
void	init_lex(t_lex *lx);
t_token	*new_token(t_tok_type type, char *str);
void	free_tokens(t_token *head);
void	flush_word(t_lex *lx);
void	append_token(t_lex *lx, t_token *tok);

/* lexer_quote.c */
void	lex_single_quote(char *input, int *i, t_lex *lx);
void	lex_double_quote(char *input, int *i, t_lex *lx, t_data *data);
void	lex_char_expand(char *input, int *i, t_lex *lx, t_data *data);
void	lex_char_plain(char *input, int *i, t_lex *lx);

/* parse.c → parse2.c */
void	add_argv(t_cmd *cmd, char *word);
void	redir_append(t_cmd *cmd, t_redir_type type, char *file, int quoted);

#endif
