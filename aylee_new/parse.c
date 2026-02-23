/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 00:00:00 by aylee             #+#    #+#             */
/*   Updated: 2026/02/23 14:01:39 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//나중에 이거 버리고 언니가 짠걸로 하기

/*
 * 토큰 종류 판별
 * ">>" → REDIR_APPEND, ">" → REDIR_OUT, "<" → REDIR_IN, "|" → REDIR_PIPE
 */
static t_redir_type	get_redir_type(char *s)
{
	if (s[0] == '>' && s[1] == '>')
		return (REDIR_APPEND);
	if (s[0] == '>')
		return (REDIR_OUT);
	if (s[0] == '<' && s[1] == '<')
		return (REDIR_HEREDOC);
	return (REDIR_IN);
}

static int	is_redir_char(char c)
{
	return (c == '>' || c == '<');
}

/*
 * redir 노드를 cmd의 redir 리스트 끝에 추가
 */
static void	add_redir(t_cmd *cmd, t_redir_type type, char *file)
{
	t_redir	*node;
	t_redir	*cur;

	node = malloc(sizeof(t_redir));
	if (!node)
		return ;
	node->type = type;
	node->file = ft_strdup(file);
	node->next = NULL;
	if (!cmd->redir)
	{
		cmd->redir = node;
		return ;
	}
	cur = cmd->redir;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

/*
 * argv 배열에 단어 추가
 * 기존 배열을 realloc 없이 새로 만들어서 연결
 */
static void	add_argv(t_cmd *cmd, char *word)
{
	int		count;
	char	**new_argv;
	int		i;

	count = 0;
	while (cmd->argv && cmd->argv[count])
		count++;
	new_argv = malloc(sizeof(char *) * (count + 2));
	if (!new_argv)
		return ;
	i = 0;
	while (i < count)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[count] = ft_strdup(word);
	new_argv[count + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
}

/*
 * 빈 t_cmd 노드 생성
 */
static t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redir = NULL;
	cmd->next = NULL;
	return (cmd);
}

/*
 * 토큰 배열에서 다음 토큰 인덱스를 찾아 리다이렉션 심볼인지 확인
 */
static int	is_redir_token(char *tok)
{
	return (ft_strncmp(tok, ">>", 3) == 0
		|| ft_strncmp(tok, ">", 2) == 0
		|| ft_strncmp(tok, "<", 2) == 0
		|| ft_strncmp(tok, "<<", 3) == 0);
}

/*
 * 입력 문자열을 공백 기준으로 토큰 배열로 분리하되,
 * ">>" "<<" ">", "<", "|" 는 붙어있어도 분리한다.
 *
 * 예: "ls>out" → ["ls", ">", "out"]
 *     "cat<<EOF" → ["cat", "<<", "EOF"]
 */
static char	**tokenize(char *input)
{
	char	**tokens;
	int		cap;
	int		count;
	char	buf[1024];
	int		buf_len;
	int		i;

	cap = 64;
	count = 0;
	buf_len = 0;
	tokens = malloc(sizeof(char *) * cap);
	if (!tokens)
		return (NULL);
	i = 0;
	while (input[i])
	{
		// 공백 → 버퍼에 쌓인 단어 저장
		if (input[i] == ' ' || input[i] == '\t')
		{
			if (buf_len > 0)
			{
				buf[buf_len] = '\0';
				tokens[count++] = ft_strdup(buf);
				buf_len = 0;
			}
			i++;
			continue ;
		}
		// ">>" 또는 "<<"
		if ((input[i] == '>' && input[i + 1] == '>')
			|| (input[i] == '<' && input[i + 1] == '<'))
		{
			if (buf_len > 0)
			{
				buf[buf_len] = '\0';
				tokens[count++] = ft_strdup(buf);
				buf_len = 0;
			}
			buf[0] = input[i];
			buf[1] = input[i + 1];
			buf[2] = '\0';
			tokens[count++] = ft_strdup(buf);
			i += 2;
			continue ;
		}
		// ">", "<", "|"
		if (is_redir_char(input[i]) || input[i] == '|')
		{
			if (buf_len > 0)
			{
				buf[buf_len] = '\0';
				tokens[count++] = ft_strdup(buf);
				buf_len = 0;
			}
			buf[0] = input[i];
			buf[1] = '\0';
			tokens[count++] = ft_strdup(buf);
			i++;
			continue ;
		}
		// 일반 문자 → 버퍼에 쌓기
		buf[buf_len++] = input[i++];
	}
	if (buf_len > 0)
	{
		buf[buf_len] = '\0';
		tokens[count++] = ft_strdup(buf);
	}
	tokens[count] = NULL;
	return (tokens);
}

/*
 * 토큰 배열을 t_cmd 연결 리스트로 변환
 *
 * "ls -la | grep foo > out.txt" →
 *   cmd[0]: argv=["ls","-la"], redir=NULL
 *   cmd[1]: argv=["grep","foo"], redir={REDIR_OUT,"out.txt"}
 */
t_cmd	*parse_pipeline(char *input)
{
	char	**tokens;
	t_cmd	*head;
	t_cmd	*cur;
	int		i;

	if (!input || !*input)
		return (NULL);
	tokens = tokenize(input);
	if (!tokens || !tokens[0])
	{
		free_split(tokens);
		return (NULL);
	}
	head = new_cmd();
	if (!head)
		return (NULL);
	cur = head;
	i = 0;
	while (tokens[i])
	{
		if (ft_strncmp(tokens[i], "|", 2) == 0)
		{
			// 파이프: 새 cmd 노드 생성
			cur->next = new_cmd();
			cur = cur->next;
		}
		else if (is_redir_token(tokens[i]))
		{
			// 리다이렉션: 다음 토큰이 파일명
			t_redir_type type = get_redir_type(tokens[i]);
			i++;
			if (tokens[i])
				add_redir(cur, type, tokens[i]);
		}
		else
		{
			// 일반 단어: argv에 추가
			add_argv(cur, tokens[i]);
		}
		i++;
	}
	free_split(tokens);
	// argv가 하나도 없고 redir도 없는 빈 노드면 NULL 반환
	if (!head->argv && !head->redir)
	{
		free_cmd_list(head);
		return (NULL);
	}
	return (head);
}

/*
 * t_cmd 리스트 전체 메모리 해제
 */
void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*next;
	t_redir	*redir;
	t_redir	*redir_next;
	int		i;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->argv)
		{
			i = 0;
			while (cmd->argv[i])
				free(cmd->argv[i++]);
			free(cmd->argv);
		}
		redir = cmd->redir;
		while (redir)
		{
			redir_next = redir->next;
			free(redir->file);
			free(redir);
			redir = redir_next;
		}
		free(cmd);
		cmd = next;
	}
}