#include "minishell_bonus.h"

void	*return_mem_alloc_error(void)
{
	ft_dprintf(STDERR_FILENO, "Error: Memory allocation failed\n");
	return (NULL);
}

void	*return_syntax_error(const char *value)
{
	ft_dprintf(STDERR_FILENO,
		"minishell: syntax error near unexpected token `%s'\n", value);
	return (NULL);
}
