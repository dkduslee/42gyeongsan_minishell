/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:40:01 by aylee             #+#    #+#             */
/*   Updated: 2025/04/08 13:53:02 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	char		*f;
	size_t		i;
	size_t		l_len;

	f = (char *)little;
	i = 0;
	l_len = ft_strlen((char *)little);
	if (l_len == 0)
		return ((char *)big);
	while (*big && i < len)
	{
		if (*big == *little)
		{
			if (ft_strncmp(big, f, l_len) == 0 && (i + l_len <= len))
				return ((char *)big);
		}
		big++;
		i++;
	}
	return (0);
}
