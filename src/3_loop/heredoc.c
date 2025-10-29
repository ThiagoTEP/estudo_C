
#include "minishell.h"
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>

static void read_heredoc(int fd, char *eof)
{
	size_t	eof_len;
	char		*line;

	eof_len = ft_strlen(eof);
	while (1)
	{
		line = readline("> ");
		if (!line || (ft_strlen(line) == eof_len
			&& !ft_strncmp(line, eof, eof_len)))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	eof_len = 0;
}

static int	make_heredoc_tmp(char *eof)
{
	int					fd;
	char				buf[64];
	static int	index;

	index++;
	ft_sprintf(buf, sizeof(buf), "/tmp/.ms_heredoc_%d", index);
	fd = open(buf, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		ft_perror(-1, "Heredoc tempfile failed\n");
		return (-1);
	}
	read_heredoc(fd, eof);
	close(fd);
	fd = open(buf, O_RDONLY, 0600);
	unlink(buf);
	return (fd);
}


int	ft_prepare_heredocs(t_token *head)
{
	int			fd;
	int			i;
	t_token	*t;

	t = head;
	while (t)
	{
		i = -1;
		while (t->redirs[++i])
		{
			if (t->redirs[i]->heredoc)
			{
				fd = make_heredoc_tmp(t->redirs[i]->eof);
				if (fd < 0)
					return (HERE_ERR);
				t->redirs[i]->heredoc_fd = fd;
			}
		}
		t = t->next;
	}
	return (SUCCESS);
}
