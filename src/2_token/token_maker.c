
#include "libft.h"
#include "minishell.h"

static bool	has_unquoted_redir(const char *s)
{
	int		i;
	bool	in_s;
	bool	in_d;

	i = 0;
	in_s = false;
	in_d = false;
	while (s[i])
	{
		if (s[i] == '\'' && !in_d)
			in_s = !in_s;
		else if (s[i] == '"' && !in_s)
			in_d = !in_d;
		else if (s[i] == '\\' && in_d && s[i + 1])
			i++;
		else if ((s[i] == '>' || s[i] == '<') && !in_s && !in_d)
			return (true);
		i++;
	}
	return (false);
}

static int	argv_size(char **raw)
{
	int	i;
	int	count;

	i = -1;
	count = 1;
	while (raw[++i])
	{
		if (!ft_strncmp(raw[i], "|", 1) || !ft_strncmp(raw[i], "&", 1)
			|| (raw[i][0] == ')' && ft_strlen(raw[i]) == 1))
			break ;
		/* usar has_unquoted_redir em vez de ft_strchr para não contar >/< dentro de quotes */
		if (has_unquoted_redir(raw[i]) || (raw[i][0] == '(' && ft_strlen(raw[i]) == 1))
			continue;
		/*
		if (ft_strchr(raw[i], '>') || ft_strchr(raw[i], '<'))
			continue ;
		*/
		count++;
	}
	return (count);
}

static char	**build_argv(char **raw)
{
	int		i;
	int		j;
	char	**argv;

	argv = ft_calloc(argv_size(raw), sizeof(char *));
	if (!argv)
		return (NULL);
	i = -1;
	j = 0;
	while (raw[++i])
	{
		if (!ft_strncmp(raw[i], "|", 1) || !ft_strncmp(raw[i], "&", 1)
		|| (!ft_strncmp(raw[i], ")", 1) && ft_strlen(raw[i]) == 1))
			break ;
		/* usar has_unquoted_redir para pular apenas redirs não-quoted */
		if (has_unquoted_redir(raw[i]) || (raw[i][0] == '(' && ft_strlen(raw[i]) == 1))
			continue;

		/*
		if (ft_strchr(raw[i], '<') || ft_strchr(raw[i], '>')
			|| (raw[i][0] == '(' && ft_strlen(raw[i]) == 1))
			continue ;
		*/
		argv[j] = ft_strdup(raw[i]);
		j++;
	}
	return (argv);
}

/*Thiago ao ler este codigo acho que talvez seja necessario tambem verificar as apas

nao alterei mas vou deixar a sugestao para ver se concorda e realmente faz diferenca*/
static void	redirs(char **raw, t_token *tok)
{
	int		i;
	int		y;
	int		total;

	i = -1;
	total = 0;
	while (raw[++i])
		/*if (has_unquoted_redir(raw[i]))*/
		if (ft_strchr(raw[i], '>') || ft_strchr(raw[i], '<'))
			total++;
	tok->redirs = malloc(sizeof(t_redir *) * (total + 1));
	i = -1;
	y = 0;
	while (raw[++i])
		/*if (has_unquoted_redir(raw[i]))*/
		if (ft_strchr(raw[i], '>') || ft_strchr(raw[i], '<'))
			tok->redirs[y++] = ft_new_redir(raw[i]);
	tok->redirs[y] = NULL;
}

static void	handle_bools(char **raw, t_token *tok)
{
	int	i;

	if (tok->prev)
		tok->if_waiter = tok->prev->if_server;
	if (tok->prev)
		tok->pipe_in = tok->prev->pipe_out;
	i = -1;
	while (raw[++i])
	{
		if (!ft_strncmp(raw[i], "|", 1) && tok->link_type == NO_LINKS)
		{
			if (raw[i + 1] && !ft_strncmp(raw[i + 1], "|", 1))
			{
				tok->if_server = true;
				tok->link_type = IF_LINK;
			}
			else
			{
				tok->link_type = PIPE_LINK;
				tok->pipe_out = true;
			}
		}
		else if (!ft_strncmp(raw[i], "&", 1))
			tok->link_type = AND_LINK;
	}
}

t_token	*ft_tokenize(char ***arr, char **ep)
{
	int		i;
	t_token	*head;
	t_token	*prev;
	t_token	*curr;

	head = NULL;
	prev = NULL;
	i = -1;
	while (arr[++i])
	{
		curr = ft_new_token();
		if (!curr)
			return (NULL);
		curr->argv = build_argv(arr[i]);
		redirs(arr[i], curr);
		curr->prev = prev;
		handle_bools(arr[i], curr);
		ft_handle_subshell(curr, arr[i]);
		if (prev)
			prev->next = curr;
		else
			head = curr;
		prev = curr;
	}
	return (ft_finish_tokens(head, ep));
}
