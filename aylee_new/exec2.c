/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 18:50:42 by aylee             #+#    #+#             */
/*   Updated: 2026/02/24 16:21:21 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	make_right_path(const char *cmd, char **path_dirs, char **full_path)
{
	char	*tmp;
	int		i;

	i = 0;
	while (path_dirs[i]) //각 경로마다 붙여서 실행가능한지 확인.
	{
		tmp = ft_strjoin(path_dirs[i], "/");
		*full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(*full_path, X_OK) == 0) //실행가능하면?
			return (1);
		free(*full_path);
		*full_path = NULL;
		i++;
	}
	return (0);
}

//실행전에 히어독 준비를 먼저시킴.
int	prepare_heredoc(t_data *data, t_cmd *cmd)
{
	t_redir	*redir;

	if (count_heredocs(cmd) > 16)
	{
		print_error_msg(data, "heredoc", "maximum here-document count exceeded", 1); //이거 나중에 형태 확인해야함.
		return (-1);
	}
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				if (collect_heredoc(redir) == -1)
					return (-1);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

int collect_heredoc(t_redir *redir)
{
	char	*tmp;
	int		fd[2];

	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			pipe(fd);
			while (1)
			{
				tmp = readline("> ");
				//ctl+d 처리를 여기에서 해야함.
				if (!tmp || ft_strncmp(tmp, redir->file, ft_strlen(redir->file)) == 0) //여기서 delimiter가 file에 들어가는 게 맞을까?
				{
					free(tmp);
					break ;
				}
				//변수 확장.
				write(fd[1], tmp, ft_strlen(tmp));
				write(fd[1], "\n", 1);
				free(tmp);
			}
			close(fd[1]);
			redir->fd = fd[0]; //fd를 구조체에 저장을 하면 어떻게 되는데?
		}
		redir = redir->next;
	}
	return (0);
}

int count_heredocs(t_cmd *cmd) //herecods가 16개 초과 경우 거르기용.
{
	int	count;
	t_redir	*redir;

	count = 0;
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
				count++;
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (count);
}