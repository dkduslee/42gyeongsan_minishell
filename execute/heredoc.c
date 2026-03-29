/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 19:17:25 by aylee             #+#    #+#             */
/*   Updated: 2026/03/29 19:00:00 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_heredocs(t_cmd *cmd)
{
	int		count;
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

void	write_line(int fd, char *line, t_data *data, int expand)
{
	char	*out;

	if (expand)
		out = expand_line(line, data);
	else
		out = ft_strdup(line);
	free(line);
	if (!out)
		return ;
	write(fd, out, ft_strlen(out));
	write(fd, "\n", 1);
	free(out);
}

void	heredoc_child(int write_fd, t_data *data, t_redir *redir, t_cmd *head)
{
	char	*line;
	int		line_count;

	signal(SIGINT, SIG_DFL);
	line_count = 0;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			signal_in_message(line_count, redir->file);
			break ;
		}
		if (ft_strncmp(line, redir->file, ft_strlen(redir->file) + 1) == 0)
		{
			free(line);
			break ;
		}
		write_line(write_fd, line, data, !redir->quoted);
		line_count++;
	}
	close(write_fd);
	exit_child(data, head, NULL, 0);
}

int	collect_heredoc(t_redir *redir, t_data *data, t_cmd *head)
{
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			if (collect_heredoc_fork(redir, data, head) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}
