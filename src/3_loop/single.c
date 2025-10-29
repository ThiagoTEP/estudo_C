
#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

// Função para executar builtins no processo principal (sem fork)
static int execute_builtin_main(t_token *tok, char **envp)
{
    return (ft_exec_builtin(tok, envp));
}

int	ft_run_single(t_token **tok, char **ep)
{
	int		status;
	pid_t	pid;

	// Se for builtin que precisa afetar o shell principal (cd, export, unset, exit)
    if ((*tok)->builtin == CD || (*tok)->builtin == EXPORT ||
        (*tok)->builtin == UNSET || (*tok)->builtin == EXIT)
    {
        status = execute_builtin_main(*tok, ep);
        *tok = (*tok)->next;
        return (status);
    }

    // Para outros comandos, fazer fork
	pid = fork();
	if (pid < 0)
		return (FORK_ERR);
	if (pid == 0)
		ft_exec_token(*tok, ep);
	else
	{
		if (waitpid(pid, &status, 0) < 0)
		{
			*tok = (*tok)->next;
			return (FORK_ERR);
		}
		*tok = (*tok)->next;
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (WTERMSIG(status) + 128);
		return (0);
	}
	return (FORK_ERR);
}
