#include "minishell_bonus.h"

void	free_str_and_nullify(char **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}