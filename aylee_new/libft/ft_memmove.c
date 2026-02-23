/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:20:11 by aylee             #+#    #+#             */
/*   Updated: 2025/04/06 10:20:40 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*temp_d;
	unsigned char	*temp_s;

	temp_d = (unsigned char *)dest;
	temp_s = (unsigned char *)src;
	if (temp_d <= temp_s)
		while (n--)
			*(temp_d++) = *(temp_s++);
	else
	{
		temp_d += (n - 1);
		temp_s += (n - 1);
		while (n--)
			*(temp_d--) = *(temp_s--);
	}
	return (dest);
}
