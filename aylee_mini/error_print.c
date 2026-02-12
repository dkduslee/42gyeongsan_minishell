/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:15:24 by aylee             #+#    #+#             */
/*   Updated: 2026/02/07 14:51:20 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//fprintf를 사용하지 않음.
void	print_error(const char *prefix, const char *message, int errnum)
{
	if (prefix)
		write(STDERR_FILENO, prefix, strlen(prefix));
	if (message)
		write(STDERR_FILENO, message, strlen(message));
	if (errnum != 0)
		write(STDERR_FILENO, ": ", 2);
	if (errnum != 0)
		write(STDERR_FILENO, strerror(errnum), strlen(strerror(errnum)));
	write(STDERR_FILENO, "\n", 1);
}

//prefix: 에러 메시지의 접두사.