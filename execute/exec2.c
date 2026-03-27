/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 18:50:42 by aylee             #+#    #+#             */
/*   Updated: 2026/03/26 19:27:00 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	make_right_path(const char *cmd, char **path_dirs, char **full_path)
{
	char	*tmp;
	int		i;

	i = 0;
	while (path_dirs[i])
	{
		tmp = ft_strjoin(path_dirs[i], "/");
		*full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(*full_path, X_OK) == 0)
			return (1);
		free(*full_path);
		*full_path = NULL;
		i++;
	}
	return (0);
}

void	signal_in_message(int line_count, char *delim)
{
	ft_putstr_fd("minishell: warning: here-document at line ", STDOUT_FILENO);
	ft_putnbr_fd(line_count, STDOUT_FILENO);
	ft_putstr_fd(" delimited by end-of-file (wanted '", STDOUT_FILENO);
	ft_putstr_fd(delim, STDOUT_FILENO);
	ft_putstr_fd("')\n", STDOUT_FILENO);
}

int	collect_heredoc_fork(t_redir *redir, char *delim, t_data *data)
{
	pid_t	pid;
	int		status;
	int		fd[2];

	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (close(fd[0]), close(fd[1]), -1);
	else if (pid == 0)
	{
		close(fd[0]);
		heredoc_child(fd[1], delim, data, !redir->quoted);
	}
	close(fd[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
		return (close(fd[0]), -1);
	redir->fd = fd[0];
	return (0);
}

int	prepare_heredoc(t_data *data, t_cmd *cmd)
{
	t_cmd	*cur;

	if (count_heredocs(cmd) > 16)
	{
		print_error_msg(data, "heredoc", \
			"maximum here-document count exceeded", 1);
		return (-1);
	}
	cur = cmd;
	while (cur)
	{
		if (collect_heredoc(cur->redir, data) == -1)
			return (-1);
		cur = cur->next;
	}
	return (0);
}
