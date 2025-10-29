#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

static bool middle_heredoc(t_redir **list, int i)
{
	if (!list || !*list)
		return (false);
	if (list[i]->heredoc == false)
	  return (false);
	while (list[++i])
	{
		if (list[i]->heredoc)
			return (true);
	}
	return (false);
}

static int	handle_in(t_redir *r)
{
	int	fd;

	fd = open(r->target, O_RDONLY);
	return (fd);
}

static int	handle_out(t_redir *r)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (r->truncate)
		flags |= O_TRUNC;
	else
		flags |= O_APPEND;
	fd = open(r->target, flags, 0644);
	return (fd);
}

static int	handle_redir(t_redir **red)
{
	int	i;
	int	fd;

	i = -1;
	while (red && red[++i])
	{
		if (red[i]->is_input)
		{
			if (red[i]->heredoc)
				fd = red[i]->heredoc_fd;
			else
				fd = handle_in(red[i]);
		}
		else
			fd = handle_out(red[i]);
		if (fd < 0)
			return (PERM_ERR);
		if (!middle_heredoc(red, i))
			dup2(fd, red[i]->fd_src);
		close(fd);
	}
	return (SUCCESS);
}

int	ft_exec_builtin(t_token *t, char **ep)
{
	(void)t;
	(void)ep;
	/*
	ft_printf("We'd now execute %s with argv:", t->argv[0]);
	for (int i = 1; t->argv[i]; i++)
	{
		if (i == 1)
			ft_printf(" {");
		ft_printf(" %s", t->argv[i]);
		if (t->argv[i + 1])
			ft_printf(" -");
		else
			ft_printf(" }\n");
	}
	if (ep)
		ft_printf("With full env\n");
	else
		ft_printf("With limited env\n");
	*/
	return (0);
}

void	ft_exec_token(t_token	*t, char **ep)
{
	if (handle_redir(t->redirs) != SUCCESS)
		exit(PERM_ERR);
	if (!t->argv || !t->argv[0])
		exit(NO_BIN);
	if (t->builtin == NOT_A_BUILTIN)
		execve(t->cmd, t->argv, ep);
	else
		exit(ft_exec_builtin(t, ep));
	ft_perror(NO_BIN, "minishell: command not found: %s\n", t->argv[0]);
	exit(NO_BIN);
}
