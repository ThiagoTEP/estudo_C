
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

static int	get_len(char *str, int i, int len)
{
	int	start;
	int	end;

	start = ft_h_skip_space(str, i + len);
	end = start;
	while (str[end] && !ft_h_isspace(str[end]) && !ft_h_operator(str[end]))
		end++;
	return (end - i);
}

static int	add_operator(char *str, int i, char ***arr, int *count)
{
	int		len;
	bool	redir;

	len = 0;
	redir = false;
	while (ft_isdigit(str[i + len]))
		len++;
	if (str[i + len] == '>' || str[i + len] == '<')
	{
		redir = true;
		len++;
		if (str[i + len] == '>' || str[i + len] == '<')
			len++;
	}
	else
		len++;
	if (redir)
		len += get_len(str, i + len, len);
	ft_h_add_word(arr, count, ft_substr(str, i, len));
	return (i + len);
}

/*Esta versão PRESERVA as aspas (inclui os caracteres ' ou " se existirem)
   e junta a substring ao token anterior se não houver espaço entre eles.*/
static int	add_quote(char *s, int i, char ***arr, int *count)
{
	int		start;
	char	q;
	char	*substr;

	start = i;
	q = s[i++];/* q é a aspa inicial */
	while (s[i])
	{
		/* em double quotes, backslash pode escapar o próximo */
		if (q == '"' && s[i] == '\\' && s[i + 1])
		{
			i += 2;
			continue ;
		}
		else if (s[i] == q)
			break ;
		i++;
	}

	/* incluir as aspas no substring (start .. i inclusive se fechamento existir) */

	if (s[i] == q)
		substr = ft_substr(s, start, i - start + 1);
	else
		substr = ft_substr(s, start, i - start);
	if (!substr)
		return (i);
	/* se a quote aparece imediatamente após outro caracter não-space/non-operator, anexar ao token anterior */
	if (start > 0 && !ft_h_isspace(s[start - 1]) && !ft_h_operator(s[start - 1]) && *count > 0)
	{
		char *prev = (*arr)[(*count) - 1];
		char *joined = ft_strjoin(prev, substr);
		free(prev);
		free(substr);
		(*arr)[(*count) - 1] = joined;
	}
	else
		ft_h_add_word(arr, count, ft_substr(s, start + 1, i - start - 1));
	if (s[i])
		i++;
	return (i);
}

static int	add_word(char *s, int i, char ***arr, int *count)
{
	int	start;

	start = i;
	if (!s[i])
		return (i);
	if (s[i] == '(' || s[i] == ')')
	{
		ft_h_add_word(arr, count, ft_substr(s, start, 1));
		return (i + 1);
	}
	while (s[i] && !ft_h_isspace(s[i]) && !ft_h_operator(s[i]))
	{
		if (s[i] == ')')
			break ;
		i++;
	}
	ft_h_add_word(arr, count, ft_substr(s, start, i - start));
	return (i);
}

static bool	detect_redir(char *s, int i)
{
	while (ft_isdigit(s[i]))
		i++;
	if (s[i] && (s[i] == '>' || s[i] == '<'))
	 return (true);
	return (false);
}

char	**ft_ms_cut_words(char *input)
{
	int		count;
	int		i;
	char	**arr;

	count = 0;
	i = 0;
	arr = NULL;
	while (input[i])
	{
		i = ft_h_skip_space(input, i);
		if (!input[i])
			break ;
		if (ft_isdigit(input[i]) && detect_redir(input, i))
			i = add_operator(input, i, &arr, &count);
		else if (ft_h_operator(input[i]))
			i = add_operator(input, i, &arr, &count);
		else if (input[i] == '\'' || input[i] == '"')
			i = add_quote(input, i, &arr, &count);
		else
			i = add_word(input, i, &arr, &count);
	}
	return (arr);
}
