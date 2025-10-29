
#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

static void	free_split(char ***arr)
{
	int	i;
	int	j;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		j = 0;
		while (arr[i][j])
		{
			free(arr[i][j]);
			j++;
		}
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	main(int ac, char **av, char **ep)
{
	static char	*input;
	static char	***split;
	t_token	*list;

	(void)ac;
	(void)av;
	while (42)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			ft_printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		split = ft_ms_split(input);
		list = ft_tokenize(split, ep);
		ft_execute(list, ep, true);
		free_split(split);
		ft_clean_list(&list);
		free(input);
	}
	rl_clear_history();
	return (0);
}
