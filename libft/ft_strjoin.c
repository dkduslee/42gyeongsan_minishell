/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 14:14:02 by aylee             #+#    #+#             */
/*   Updated: 2025/04/06 14:14:04 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strcat(char *result, char *s1, char *s2)
{
	while (*s1)
		*result++ = *s1++;
	while (*s2)
		*result++ = *s2++;
	*result = '\0';
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s1_temp;
	char	*s2_temp;
	int		s1_len;
	int		s2_len;
	char	*string;

	s1_temp = (char *)s1;
	s2_temp = (char *)s2;
	s1_len = ft_strlen(s1_temp);
	s2_len = ft_strlen(s2_temp);
	string = (char *)malloc(s1_len + s2_len + 1);
	if (!string)
		return (0);
	ft_strcat(string, s1_temp, s2_temp);
	return (string);
}
