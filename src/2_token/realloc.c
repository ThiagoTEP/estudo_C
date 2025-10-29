
#include "minishell.h"
#include <stdlib.h>

static void	smart_copy(void *from, void *to, size_t o, size_t n)
{
	size_t			i;
	size_t			size;
	unsigned char	*src;
	unsigned char	*dst;

	src = (unsigned char *)from;
	dst = (unsigned char *)to;
	if (o < n)
		size = o;
	else
		size = n;
	i = 0;
	while (i < size)
	{
		dst[i] = src[i];
		i++;
	}
}

void	*ft_realloc(void *old, size_t og, size_t target)
{
	void	*new;

	if (target <= 0)
	{
		free(old);
		return (NULL);
	}
	new = ft_calloc(1, target);
	if (!new)
		return (NULL);
	if (old)
	{
		smart_copy(old, new, og, target);
		free(old);
	}
	return (new);
}
