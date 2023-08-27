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

void	*free_and_return_null(void *ptr)
{
	if (ptr)
		free(ptr);
	ptr = NULL;
	return (NULL);
}

void	*free_2_and_return_null(void *ptr1, void *ptr2)
{
	if (ptr1)
		free(ptr1);
	if (ptr2)
		free(ptr2);
	ptr1 = NULL;
	ptr2 = NULL;
	return (NULL);
}
