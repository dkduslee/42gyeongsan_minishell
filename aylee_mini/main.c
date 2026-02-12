#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_shell
{
	char			**envp;
	char			*av; //원본
	struct s_input	*tokens; //파싱된 거
	int				stdin_backup; //리다이렉션에 씀
	int				stdout_backup;
	int				exit_status; //$?에 쓸거
} t_shell;

void	init_shell(t_shell *shell, char **envp)
{
	shell->envp = envp;
	shell->exit_status = 0;
}

int	parse_and_execute(t_shell *shell, char *line)
{
	(void)shell;
	if (ft_strcmp(line, "exit") == 0)
	{
		shell->exit_status = 0;
		return 1;
	}
	if ()
	// 파싱된 거 기반으로 envp랑 비교구문.
	printf("입력받은 내용: %s\n", line);
	return 0;
}

void	cleanup_shell(t_shell *shell)
{
	(void)shell;
	// 셸 종료 시 필요한 정리 작업 수행
}
int main(int argc, char **argv, char **envp)
{
	t_shell shell; //arg 총관리자. envp 
	char    *line;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	while (1)
	{
    	line = readline("minishell$ ");
    	if (!line)
        	break;
	    if (*line)
    	    add_history(line);
	    if (parse_and_execute(&shell, line))
    	    break;
 	   free(line);
	   line = NULL;
	}
	cleanup_shell(&shell);
	return (shell.exit_status);
}