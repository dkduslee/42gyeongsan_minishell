/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 19:01:21 by aylee             #+#    #+#             */
/*   Updated: 2026/03/02 19:02:42 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int is_valid_num(const char *str)
{
	int i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int long_range_check(char *str)
{
	char *max;
	char *min;
	int i;
	int len;

	max = "9223372036854775807";
	min = "9223372036854775808";
	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
	len = 0;
	while (str[i + len])
		len++;
	if (len < 19)
		return (1);
	if (len > 19)
		return (0);
	if (str[0] == '-')
		return (ft_strncmp(str + i, min, 19) <= 0);
	return (ft_strncmp(str + i, max, 19) <= 0);
}

long long str_to_ll(char *str)
{
	long long result;
	int sign;
	int i;

	result = 0;
	sign = 1;
	i = 0;
	if (str[0] == '-' || str[0] == '+')
	{
		if (str[0] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
		result = result * 10 + (str[i++] - '0');
	return (result * sign);
}

int builtin_exit(t_data *data, char **args)
{
	int status;

	printf("exit\n");
	if (!args || !args[0])
	{
		status = data->exit_status;
		clean_up(data);
		exit(status);
	}
	if (!is_valid_num(args[0]) || !long_range_check(args[0]))
	{
		print_error_msg(data, "exit", "numeric argument required", 2);
		clean_up(data);
		exit(2);
	}
	if (args[1])
	{
		print_error_msg(data, "exit", "too many arguments", 1);
		return (1);
	}
	status = str_to_ll(args[0]);
	clean_up(data);
	exit((int)(status % 256));
}