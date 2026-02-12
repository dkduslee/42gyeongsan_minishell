/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:49:17 by aylee             #+#    #+#             */
/*   Updated: 2025/04/06 15:53:13 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*result;
	char			*s_temp;
	unsigned int	i;

	i = 0;
	s_temp = (char *)s;
	result = (char *)malloc(ft_strlen(s_temp) + 1);
	if (!result)
		return (0);
	while (s_temp[i])
	{
		result[i] = f(i, s_temp[i]);
		i++;
	}
	result[i] = 0;
	return (result);
}
