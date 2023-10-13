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

char	*join_1st_and_cleanup(char **malloced_str1, char *malloced_str2)
{
	char	*new_str;

	if (!malloced_str1 || !malloced_str2 || !*malloced_str1)
		return (NULL);
	new_str = ft_strjoin(*malloced_str1, malloced_str2);
	free(*malloced_str1);
	*malloced_str1 = NULL;
	if (!new_str)
		return (NULL);
	return (new_str);
}

int success_or_mem_error(int exit_status)
{
	if (exit_status == MISUSE)
		print_mem_alloc_error();
	return (exit_status);
}

void	*error_and_exit_code_false(t_token_flags *flags)
{
	flags->has_exit_code = false;
	return (return_mem_alloc_error());
}

void	*null_exit_code_false(t_token_flags *flags)
{
	flags->has_exit_code = false;
	return (NULL);
}

void	*null_exit_code_false_free_string(t_token_flags *flags)
{
	flags->has_exit_code = false;
	return (free_and_return_null(flags->string));
}
