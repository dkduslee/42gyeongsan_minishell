/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sub_built.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 17:36:58 by aylee             #+#    #+#             */
/*   Updated: 2026/03/29 18:11:34 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || !arg[1])
		return (0);
	i = 1;
	while (arg[i])
		if (arg[i++] != 'n')
			return (0);
	return (1);
}

int	builtin_echo(t_data *data, char **args)
{
	int	i;
	int	newline;

	(void)data;
	newline = 1;
	i = 0;
	while (args && args[i] && is_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args && args[i])
	{
		if (ft_strncmp(args[i], "$?", 3) == 0)
			printf("%d", data->exit_status);
		else
			printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	builtin_cd(t_data *data, char **args)
{
	const char	*path;
	t_env		*home_node;
	t_env		*pwd_node;
	char		*cwd;

	if (!args || args[0] == NULL)
	{
		home_node = find_env_node(data->env, "HOME");
		if (home_node)
			path = home_node->value;
		else
			return (print_error_msg(data, "cd", "HOME not set", 1), 1);
	}
	else
		path = args[0];
	if (chdir(path) != 0)
		return (print_error_msg(data, "cd", strerror(errno), 1), 1);
	pwd_node = find_env_node(data->env, "PWD");
	if (pwd_node)
		set_env_var(data, "OLDPWD", pwd_node->value);
	cwd = getcwd(NULL, 0);
	set_env_var(data, "PWD", cwd);
	return (free(cwd), 0);
}

int	execute_builtin(t_data *data, t_cmd *cmd)
{
	if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
		return (builtin_echo(data, cmd->argv));
	else if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
		return (builtin_cd(data, cmd->argv));
	else if (ft_strncmp(cmd->cmd, "pwd", 4) == 0)
		return (builtin_pwd(data));
	else if (ft_strncmp(cmd->cmd, "export", 7) == 0)
		return (builtin_export(data, cmd->argv));
	else if (ft_strncmp(cmd->cmd, "unset", 6) == 0)
		return (builtin_unset(data, cmd->argv));
	else if (ft_strncmp(cmd->cmd, "exit", 5) == 0)
		return (builtin_exit(data, cmd, cmd->argv));
	else if (ft_strncmp(cmd->cmd, "env", 4) == 0)
		return (builtin_env(data, cmd->argv));
	return (-1);
}
