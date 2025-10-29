
#include "minishell.h"
#include <sys/wait.h>

static void	subtract_depth(t_token *t)
{
	int			depth;

	depth = t->subshell;
	while (t)
	{
		if (t->subshell >= depth)
			t->subshell--;
		if (t->subshell_end && t->subshell == depth - 1)
		{
			if (t->next)
				t->next->cutoff = true;
			break ;
		}
		t = t->next;
	}

}

int	ft_run_subshell(t_token **tok, char **ep)
{
	int		ret;

	//TODO: SHLVL++
	subtract_depth(*tok);
	ret = ft_execute(*tok, ep, false);
	while (*tok && !(*tok)->cutoff)
		*tok = (*tok)->next;
	if (*tok) {
		(*tok)->cutoff = false;
	}
	//TODO: SHLVL--
	return (ret);
}

