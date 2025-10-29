
#include "minishell.h"
#include <unistd.h>

static int	get_fd(char *r)
{
	int		fd;

	if (ft_strchr(r, '>'))
		fd = STDOUT_FILENO;
	else
		fd = STDIN_FILENO;
	if (ft_isdigit(r[0]))
		fd = ft_atoi(r);
	return (fd);
}

static char	*get_target(char *r)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (r[i] != '>' && r[i] != '<')
		i++;
	if (r[i + 1] == '>' || r[i + 1] == '<')
		i++;
	i++;
	i = ft_h_skip_space(r, i);
	while (r[i + len] && !ft_h_isspace(r[i + len]) && !ft_h_operator(r[i + len])
		&& r[i + len] != '(' && r[i + len] != ')')
		len++;
	return (ft_substr(r, i, len));
}

static void	set_bools(char *r, t_redir *new)
{
	int	i;

	i = 0;
	while (r[i] != '>' && r[i] != '<')
		i++;
	if (r[i] == '<')
	{
		new->is_input = true;
		if (r[i + 1] == '<')
			new->heredoc = true;
	}
	else
	{
		if (r[i + 1] != '>')
			new->truncate = true;
	}
}

t_redir *ft_new_redir(char *raw)
{
	t_redir	*new;

	new = ft_calloc(sizeof(t_redir), 1);
	if (!new)
		return (NULL);
	new->fd_src = get_fd(raw);
	new->heredoc_fd = -1;
	set_bools(raw, new);
	if (new->heredoc)
		new->eof = get_target(raw);
	else
		new->target = get_target(raw);
	return (new);
}
