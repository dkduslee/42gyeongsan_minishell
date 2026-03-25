/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 19:08:10 by aylee             #+#    #+#             */
/*   Updated: 2026/03/02 19:16:47 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command_path(char *cmd, t_env *env)
{
	t_env	*path_node;
	char	**dirs;
	char	*full_path;

	full_path = NULL;
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_node = find_env_node(env, "PATH");
	if (!path_node || !path_node->value)
		return (NULL);
	dirs = ft_split(path_node->value, ':');
	if (!dirs)
		return (NULL);
	if (make_right_path(cmd, dirs, &full_path))
		return (free_split(dirs), full_path);
	return (free_split(dirs), NULL);
}

char	**get_execve_args(t_cmd *cmd) //합치는 코드
{
	char	**args;
	int		argc;
	int		i;

	argc = 0;
	i = 0;
	while (cmd->argv && cmd->argv[argc])
		argc++;
	args = malloc(sizeof(char *) * (argc + 2));
	if (!args)
		return (NULL);
	args[0] = ft_strdup(cmd->cmd);
	if (!args[0])
		return (free(args), NULL);
	while (i < argc)
	{
		args[i + 1] = ft_strdup(cmd->argv[i]);
		if (!args[i + 1])
			return (free_split(args), NULL);
		i++;
	}
	args[argc + 1] = NULL;
	return (args);
}
