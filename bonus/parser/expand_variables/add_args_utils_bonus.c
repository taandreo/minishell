#include "minishell_bonus.h"

void	*null_free_args_misuse(t_arguments *args, t_arguments *curr_args,
			t_string *str_list, t_vars *vars)
{
	if (args)
		free_arguments(args);
	if (curr_args)
		free_arguments(curr_args);
	free_string(str_list);
	vars->state.status = MISUSE;
	vars->state.error = true;
	return (NULL);
}

void	walk_last_args(t_arguments *last_args)
{
	while (last_args->next)
		last_args = last_args->next;
}

void	*free_resources_and_return_null(DIR *dir, t_string *result_head)
{
	closedir(dir);
	free_string(result_head);
	return (NULL);
}
