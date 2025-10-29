
#include "minishell.h"
#include "../../inc/libft.h"
#include "../../inc/ft_printf.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static char **clean_return(char **arr, int ignore)
{
	int	i;

	i = 0;
	while (arr[i] && i != ignore)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (NULL);
}

static char	**extract_path(char **ep)
{
	int	i;

	i = -1;
	while (ep[++i])
	{
		if (!ft_strncmp("PATH=/", ep[i], 6))
			break ;
	}
	if (ep[i])
		return (ft_split(ep[i] + 5, ':'));
	else
		return (NULL);
}

static char	**add_bin_to_path(char **path, char *bin)
{
	int		i;
	size_t	len;
	size_t	temp_len;

	i = -1;
	len = ft_strlen(bin) + 2;
	while (path[++i])
	{
		temp_len = ft_strlen(path[i]);
		path[i] = (char *)ft_realloc(path[i], temp_len, temp_len + len);
		if (!path[i])
			return (clean_return(path, i));
		ft_sprintf(path[i], len + temp_len, "%s/%s", path[i], bin);
	}
	return (path);
}

static char	*give_valid_path(char **bins)
{
	int	i;

	i = 0;
	while (bins[i])
	{
		if (!access(bins[i], X_OK))
			break ;
		i++;
	}
	if (bins[i])
		return (ft_strdup(bins[i]));
	else
		return (NULL);
}

char	*ft_get_bin(char *program, char **envp)
{
	char	*bin_path;
	char	**paths;

	if (!program)
		return (NULL);
	if (ft_strchr(program, '/'))
	{
		if (!access(program, X_OK))
			return (program);
		return (NULL);
	}
	paths = extract_path(envp);
	if (!paths)
		return (NULL);
	paths = add_bin_to_path(paths, program);
	if (!paths)
		return (NULL);
	bin_path = give_valid_path(paths);
	clean_return(paths, -1);
	if (!bin_path)
		return (NULL);
	return (bin_path);
}

