#include "minishell.h"
#include <stdlib.h>

static void	undo_charpp(char **arr)
{
	int	i;

	if (!arr || !*arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static void	undo_redirs(t_redir **red)
{
	int	i;

	i = 0;
	if (!red)
		return ;
	while (red[i])
	{
		if (red[i]->target)
			free(red[i]->target);
		if (red[i]->eof)
			free(red[i]->eof);
		free(red[i]);
		i++;
	}
	free(red);
}

void	ft_clean_list(t_token **l)
{
	t_token	*c;

	c = *l;
	while (*l)
	{
		undo_charpp((*l)->argv);
		undo_redirs((*l)->redirs);
		c = (*l)->next;
		free(*l);
		*l = c;
	}
}
