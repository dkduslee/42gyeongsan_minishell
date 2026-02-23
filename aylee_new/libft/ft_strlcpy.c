/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:30:35 by aylee             #+#    #+#             */
/*   Updated: 2025/04/06 10:22:24 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	int	len;

	len = ft_strlen((char *)src);
	if (size == 0)
		return (len);
	while (*src && size-- > 1)
		*dest++ = *src++;
	*dest = 0;
	return (len);
}
