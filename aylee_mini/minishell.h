/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <aylee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 14:46:57 by aylee             #+#    #+#             */
/*   Updated: 2026/02/12 13:22:05 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include "../aylee/pipex/libft/libft.h"

typedef struct s_env
{
	char 				*key;
	char 				*value;
	struct s_env 		*next;
} t_env;

typedef struct s_data
{
	t_env	*env;
	int		exit_status; //프로세스의 종료상태.
} t_data;

typedef struct s_input //파싱부 형태 예시
{
	char			*cnt;
	char			*type;
	int				len;
	struct s_input	*next;
	//여기에 ""이 있는지 판단하는 것도 넣기.
} t_input;

t_env *create_env_node(const char *key, const char *value);
void free_env_node(t_env *node);
t_env *find_env_node(t_env *head, const char *key);
t_env *add_env_node(t_env **head, const char *key, const char *value);
t_env *parse_env(char **envp);
void free_env_list(t_env *head);
void delete_env(t_env *head, char *key);
void print_env_list(t_env *head);
t_data	*init_data(char **envp);

#endif