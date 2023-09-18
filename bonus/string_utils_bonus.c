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
		return (return_mem_alloc_error());
	return (new_str);
}

char	*join_and_cleanup_1st(char **malloced_str1, char *str2)
{
	char	*new_str;

	if (!malloced_str1 || !*malloced_str1)
		return (NULL);
	new_str = ft_strjoin(*malloced_str1, str2);
	free(*malloced_str1);
	*malloced_str1 = NULL;
	if (!new_str)
		return (return_mem_alloc_error());
	return (new_str);
}
