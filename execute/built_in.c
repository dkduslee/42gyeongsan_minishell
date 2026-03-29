/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 14:46:49 by aylee             #+#    #+#             */
/*   Updated: 2026/03/29 18:09:10 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_data *data)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	print_error_msg(data, "pwd", strerror(errno), 1);
	return (1);
}

int	builtin_unset(t_data *data, char **args)
{
	int	i;

	if (!args || args[0] == NULL)
		return (0);
	i = 0;
	while (args[i])
	{
		delete_env(&data->env, args[i]);
		i++;
	}
	return (0);
}

int	builtin_env(t_data *data, char **args)
{
	if (args && args[0] != NULL)
	{
		print_error_msg(data, "env", "too many arguments", 1);
		return (1);
	}
	print_env_list(data->env);
	return (0);
}
