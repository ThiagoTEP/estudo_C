
#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

static void	pipe_child(t_token *t, int in, int out, char **ep)
{
	if (in != STDIN_FILENO)
	{
		dup2(in, STDIN_FILENO);
		close(in);
	}
	if (out != STDOUT_FILENO)
	{
		dup2(out, STDOUT_FILENO);
		close(out);
	}
	ft_exec_token(t, ep);
}

static int	pipe_loop(t_token **curr, char **ep, int in_fd)
{
	int		fd[2];
	pid_t	pid;

	while ((*curr)->link_type == PIPE_LINK
	&& !(*curr)->cutoff)
	{
		if (pipe(fd) == -1)
			return (ft_perror(PIPE_ERR, NULL));
		pid = fork();
		if (pid == -1)
			return (ft_perror(FORK_ERR, NULL));
		if (pid == 0)
			pipe_child(*curr, in_fd, fd[1], ep);
		close(fd[1]);
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		in_fd = fd[0];
		*curr = (*curr)->next;
	}
	return (in_fd);
}

static int	wait_last_pid(pid_t last_pid)
{
	int		ret;
	int		status;
	pid_t	pid;

	ret = 0;
	while (1)
	{
		pid = wait(&status);
		if (pid <= 0)
			break ;
		if (pid == last_pid)
			ret = status;
	}
	if (WIFEXITED(ret))
		return (WEXITSTATUS(ret));
	else if (WIFSIGNALED(ret))
		return (WTERMSIG(ret) + 128);
	return (ret);
}


/*CONFERIR Thiago, alterei de segunda quando conversamos*/
int	ft_run_pipeline(t_token **curr, char **ep)
{
	int		ret;
	int		in_fd;
	pid_t	pid;

	in_fd = pipe_loop(curr, ep, STDIN_FILENO);
	if (in_fd < 0)
		return (OPEN_ERR);

	// Se for builtin que precisa afetar o shell principal (cd, export, unset, exit)
    if ((*curr)->builtin == CD || (*curr)->builtin == EXPORT ||
        (*curr)->builtin == UNSET || (*curr)->builtin == EXIT)
    {
        ret = execute_builtin_main(*curr, ep);
        *curr = (*curr)->next;
        return (ret);
    }

    // Para outros comandos, fazer fork


	pid = fork();
	if (pid == -1)
		return (ft_perror(FORK_ERR, NULL));
	if (pid == 0)
		pipe_child(*curr, in_fd, STDOUT_FILENO, ep);
	close(in_fd);
	ret = wait_last_pid(pid);
	*curr = (*curr)->next;
	return (ret);
}
