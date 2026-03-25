/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 14:13:40 by aylee             #+#    #+#             */
/*   Updated: 2025/04/06 14:19:31 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	num_len(long int nbr)
{
	int	i;

	i = 1;
	if (nbr < 0)
	{
		i++;
		nbr *= -1;
	}
	while (nbr / 10 != 0)
	{
		i++;
		nbr /= 10;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int			j;
	long int	nb;
	int			len;
	char		*result;

	j = 1;
	nb = n;
	len = num_len(n);
	result = malloc(len + 1);
	if (nb < 0)
	{
		result[0] = '-';
		nb *= -1;
	}
	if (nb == 0)
		result[0] = '0';
	while (nb)
	{
		result[len - j] = (nb % 10) + '0';
		j++;
		nb /= 10;
	}
	result[len] = '\0';
	return (result);
}
