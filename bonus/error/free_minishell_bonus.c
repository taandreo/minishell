#include "minishell_bonus.h"

void	free_mini_line(t_vars *vars)
{
	if (vars->parse_tree)
	{
		free_command(*vars->parse_tree);
		*vars->parse_tree = NULL;
	}
	free_token_list(vars->tokens);
	if (vars->prompt && *vars->prompt)
	{
		free(*vars->prompt);
		*vars->prompt = NULL;
	}
	if (vars->args)
	{
		free(vars->args);
		vars->args = NULL;
	}
	if (vars->nice_prompt)
		free(vars->nice_prompt);
}

void	free_minishell(t_vars *vars)
{
	free_mini_line(vars);
	free_all_envs();
}