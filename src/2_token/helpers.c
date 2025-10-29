
#include "minishell.h"

bool	ft_h_isspace(char c)
{
	return (c == ' ' ||
		 (c >= 9 && c <= 13));
}

bool	ft_h_operator(char c)
{
	return (c == '&' || c == '|' || c == '<' || c == '>');
}

int ft_h_skip_space(char *str, size_t i)
{
	while (str[i] && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		i++;
	return (i);
}

void	ft_h_add_word(char ***arr, int *count, char *word)
{
	*arr = ft_realloc(*arr,
		sizeof(char *) * (*count + 1),
		sizeof(char *) * (*count + 2));
	if (!*arr)
		return ;
	(*arr)[(*count)++] = word;
	(*arr)[(*count)] = NULL;
}
