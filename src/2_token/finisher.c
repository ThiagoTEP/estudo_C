
#include "minishell.h"

t_token	*ft_finish_tokens(t_token *head, char **ep)
{
	t_token	*tmp;

	tmp = head;
	while (tmp)
	{
		tmp->builtin = ft_check_builtin(tmp->argv[0]);
		if (tmp->argv && tmp->argv[0])
		{
			tmp->cmd = ft_get_bin(tmp->argv[0], ep);
			if (!tmp->cmd)
				tmp->status = NO_BIN;
			else
				tmp->status = SUCCESS;
		}
		else
			tmp->status = NO_BIN;
		if (tmp->builtin != NOT_A_BUILTIN)
			tmp->status = SUCCESS;
		tmp = tmp->next;
	}
	return (head);
}
