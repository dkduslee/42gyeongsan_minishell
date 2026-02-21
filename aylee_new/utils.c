/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 00:00:00 by aylee             #+#    #+#             */
/*   Updated: 2026/02/21 17:45:48 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*s1_t;
	unsigned char	*s2_t;

	s1_t = (unsigned char *)s1;
	s2_t = (unsigned char *)s2;
	while (n--)
	{
		if (*s1_t != *s2_t)
			return (*s1_t - *s2_t);
		s1_t++;
		s2_t++;
	}
	return (0);
}

size_t	ft_strlen(const char *s)
{
	size_t		len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}
void	ft_strcat(char *result, char *s1, char *s2)
{
	while (*s1)
		*result++ = *s1++;
	while (*s2)
		*result++ = *s2++;
	*result = '\0';
}

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	len_dest;
	size_t	len_src;
	size_t	i;

	i = 0;
	len_dest = ft_strlen(dest);
	len_src = ft_strlen((char *)src);
	if (len_dest > size)
		return (size + len_src);
	while (src[i] && (len_dest + i + 1) < size)
	{
		dest[len_dest + i] = src[i];
		i++;
	}
	dest[len_dest + i] = '\0';
	return (len_dest + len_src);
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

char	*ft_strdup(const char *s)
{
	char	*dup;
	size_t	len;
	size_t	i;

	if (!s)
		return (NULL);
	len = 0;
	while (s[len])
		len++;
	dup = (char *)malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

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

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

void	clean_up(t_data *data)
{
	if (data)
	{
		if (data->env)
			free_env_list(data->env);
		free(data);
	}
}

static int	count_words(char const *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static char	*get_word(char const *s, char c)
{
	int		len;
	char	*word;
	int		i;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	word = (char *)malloc(len + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		i;

	if (!s)
		return (NULL);
	result = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
		{
			result[i] = get_word(s, c);
			if (!result[i])
				return (NULL);
			i++;
			while (*s && *s != c)
				s++;
		}
	}
	result[i] = NULL;
	return (result);
}

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

/*
 * t_env 연결 리스트를 execve용 char** 배열로 변환
 * 반환값: {"KEY=VALUE", ..., NULL} 형태 (호출자가 free_split으로 해제)
 */
char	**env_to_array(t_env *env)
{
	t_env	*cur;
	char	**arr;
	char	*tmp;
	int		count;
	int		i;

	count = 0;
	cur = env;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	arr = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	cur = env;
	i = 0;
	while (cur)
	{
		if (cur->value)
		{
			tmp = ft_strjoin(cur->key, "=");
			arr[i] = ft_strjoin(tmp, cur->value);
			free(tmp);
		}
		else
			arr[i] = ft_strdup(cur->key);
		i++;
		cur = cur->next;
	}
	arr[i] = NULL;
	return (arr);
}
