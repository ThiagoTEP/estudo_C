
#include "minishell.h"


void	debug_struct(t_token *c)
{
	ft_printf("Debugging now t_token with cmd: %s\n", c->argv[0]);
	if (c->status == NO_BIN)
		ft_printf("Broken token with no executable\n");
	if (c->builtin != NOT_A_BUILTIN)
		ft_printf("This token is a builtin\n");
	for (int i = 0; c->argv[i]; i++)
	{
		if (!i)
			ft_printf("{ ");
		ft_printf(c->argv[i]);
		if (c->argv[i + 1])
			ft_printf(" ");
		else
			ft_printf(" }\n");
	}
}

int	ft_execute(t_token *head, char **ep, bool newhere)
{
	int		ret;
	t_token	*curr;

	curr = head;
	ret = 0;
	if (newhere && ft_prepare_heredocs(head) != SUCCESS)
		return (HERE_ERR);
	while (curr && !curr->cutoff)
	{
		debug_struct(curr);
		if ((curr->prev && curr->prev->link_type == AND_LINK && ret != 0)
			|| (curr->prev && curr->prev->link_type == IF_LINK && ret == 0))
		{
			curr = curr->next;
			continue ;
		}
		if (curr->subshell)
			ret = ft_run_subshell(&curr, ep);
		else if (curr->link_type == PIPE_LINK)
			ret = ft_run_pipeline(&curr, ep);
		else
			ret = ft_run_single(&curr, ep);
	}
	return (ret);
}
