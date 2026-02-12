/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:25:41 by aylee             #+#    #+#             */
/*   Updated: 2025/04/06 10:26:50 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strchr(const char *s, int n)
{
	char			*s_temp;
	unsigned char	nb;

	s_temp = (char *)s;
	nb = (unsigned char)n;
	while (*s_temp != '\0')
	{
		if (*s_temp == nb)
			return (s_temp);
		s_temp++;
	}
	if (nb == 0)
		return (s_temp);
	return (0);
}
