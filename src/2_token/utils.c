
#include "libft.h"
#include "minishell.h"

t_token	*ft_new_token(void)
{
	t_token	*new;

	new = ft_calloc(1, sizeof(t_token));
	if (!new)
		return (NULL);
	new->link_type = NO_LINKS;
	new->if_waiter = false;
	new->pipe_in = false;
	new->pipe_out = false;
	new->argv = NULL;
	new->prev = NULL;
	new->next = NULL;
	new->redirs = NULL;
	return (new);
}
