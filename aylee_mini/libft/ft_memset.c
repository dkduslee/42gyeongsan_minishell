/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:10:42 by aylee             #+#    #+#             */
/*   Updated: 2025/04/06 16:01:13 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*result;
	size_t			i;

	result = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		result[i] = (unsigned char)c;
		i++;
	}
	return ((void *)result);
}
