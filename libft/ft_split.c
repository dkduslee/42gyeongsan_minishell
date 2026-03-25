/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 14:13:52 by aylee             #+#    #+#             */
/*   Updated: 2025/04/06 14:13:53 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	is_set(char cha, char c)
{
	if (cha == c)
		return (1);
	if (cha == 0)
		return (1);
	return (0);
}

int	str_count(const char *str, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (str[i] && is_set(str[i], c))
		i++;
	while (str[i])
	{
		if (!is_set(str[i], c) && is_set(str[i + 1], c))
			count++;
		i++;
	}
	return (count);
}

char	*make_str(const char *str, char c, int *i)
{
	char	*re;
	int		j;
	int		k;
	int		len;

	while (str[*i] && is_set(str[*i], c))
		(*i)++;
	j = *i;
	while (str[*i])
	{
		if (!is_set(str[*i], c) && is_set(str[*i + 1], c))
			break ;
		(*i)++;
	}
	(*i)++;
	len = *i - j;
	re = (char *)malloc(len + 1);
	k = 0;
	while (j < *i)
		re[k++] = str[j++];
	re[k] = '\0';
	return (re);
}

char	**ft_split(const char *str, char c)
{
	int		count;
	int		i;
	int		index;
	char	**string;

	i = 0;
	index = 0;
	count = str_count(str, c);
	string = (char **)malloc(sizeof(char *) * (count + 1));
	if (!string)
		return (0);
	while (i < count)
	{
		string[i] = make_str(str, c, &index);
		i++;
	}
	string[i] = 0;
	return (string);
}
