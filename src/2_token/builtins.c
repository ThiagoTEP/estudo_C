
#include "minishell.h"


static int	cb_end(char *p, int len)
{
	if (len == 5)
		if (!ft_strncmp(p, "unset", len))
			return (UNSET);
	if (len == 6)
		if (!ft_strncmp(p, "export", len))
			return (EXPORT);
	return (NOT_A_BUILTIN);
}

int	ft_check_builtin(char *program)
{
	int	len;

	len = ft_strlen(program);

	if (len < 2 || len > 6)
		return (NOT_A_BUILTIN);
	if (len == 2)
		if (!ft_strncmp(program, "cd", len))
			return (CD);
	if (len == 3)
	{
		if (!ft_strncmp(program, "pwd", len))
			return (PWD);
		else if (!ft_strncmp(program, "env", len))
			return (ENV);
	}
	else if (len == 4)
	{
		if (!ft_strncmp(program, "echo", len))
			return (ECHO);
		else if (!ft_strncmp(program, "exit", len))
			return (EXIT);
	}
	return (cb_end(program, len));
}

