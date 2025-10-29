#include <stdio.h>
#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

static int	jump_quote(char *str, char q, int i)
{
	i++;
	while (str[i])
	{
		if (q == '"' && str[i] == '\\' && str[i + 1])
		{
			i += 2;
			continue ;
		}
		else if (str[i] == q)
			break ;
		i++;
	}
	return (i);
}

static int	count_cmds(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	while (str[i])
	{
		if (str[i] == '\'' || (str[i] == '"' && i >= 1 && str[i - 1] != '\\'))
			i = jump_quote(str, str[i], i);
		if (str[i] == '|' || (str[i] == '&' && str[i + 1] == '&'))
		{
			count++;
			if (str[i] == '|' && str[i + 1] == '|')
				i++;
		}
		i++;
	}
	return (count);
}

static int	jump_substr(char *str, int i)
{
	while (str[i])
	{
		if (str[i] == '\'' || (str[i] == '"' && i >= 1 && str[i - 1] != '\\'))
			i = jump_quote(str, str[i], i);
		if (str[i] == '&' && str[i + 1] == '&')
			return (i + 2);
		if (str[i] == '|')
		{
			if (str[i + 1] == '|')
				return (i + 2);
			return (i + 1);
		}
		i++;
	}
	return (i);
}

char	***ft_ms_split(char *input)
{
	int		i;
	int		ret_i;
	int		start;
	char	*temp;
	char	***ret;

	ret = ft_calloc(count_cmds(input) + 1, sizeof(char **));
	i = 0;
	ret_i = 0;
	start = 0;
	while (input[i])
	{
		i = jump_substr(input, i);
		temp = ft_substr(input, start, i - start);
		ret[ret_i++] = ft_ms_cut_words(temp);
		free(temp);
		start = i;
	}
	ret[ret_i] = NULL;
	return (ret);
}
