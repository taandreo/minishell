#include "minishell_bonus.h"

char	*join_and_cleanup(char **malloced_str1, char **malloced_str2)
{
	char	*new_str;

	if (!malloced_str1 || !malloced_str2 || !*malloced_str1 || !*malloced_str2)
		return (NULL);
	new_str = ft_strjoin(*malloced_str1, *malloced_str2);
	free(*malloced_str1);
	*malloced_str1 = NULL;
	free(*malloced_str2);
	*malloced_str2 = NULL;
	if (!new_str)
		return (NULL);
	return (new_str);
}

int success_or_mem_error(int exit_status)
{
	if (exit_status == MISUSE)
		ft_dprintf(STDERR_FILENO, "Error: Memory allocation failed\n");
	return (exit_status);
}
