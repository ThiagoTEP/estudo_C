
#include "minishell.h"

void	ft_handle_subshell(t_token *t, char **raw)
{
	int			i;
	t_token	*tmp;

	i = -1;
	while (raw[++i])
	{
		if (!ft_strncmp(raw[i], "(", 1))
		{
			t->subshell += 1;
			tmp = t;
			while (tmp->next)
			{
				tmp->next->subshell += 1;
				tmp = tmp->next;
			}
		}
		else if (!ft_strncmp(raw[i], ")", 1))
		{
			t->subshell_end = true;
			tmp = t;
			while (tmp->next)
			{
				tmp->next->subshell -= 1;
				tmp = tmp->next;
			}
		}
	}
}
