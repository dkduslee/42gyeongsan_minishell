/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:42:07 by aylee             #+#    #+#             */
/*   Updated: 2025/04/06 10:43:56 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*temp;

	if (size == 0 || nmemb == 0)
		return (malloc(1));
	if (nmemb > (size_t)-1 / size)
		return (0);
	temp = malloc(size * nmemb);
	if (!temp)
		return (0);
	ft_bzero(temp, nmemb * size);
	return (temp);
}
