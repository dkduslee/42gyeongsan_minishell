/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoyeoki <seoyeoki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 00:00:00 by seoyeoki          #+#    #+#             */
/*   Updated: 2026/03/25 00:00:00 by seoyeoki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_int.h"

void	err_syntax_token(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
}

void	err_unclosed_quote(void)
{
	ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
}

int	syntax_err(t_data *data, char *token)
{
	err_syntax_token(token);
	data->exit_status = 2;
	return (-1);
}
