/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_minishell_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:33:13 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/15 23:33:16 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_mini_line(t_vars *vars)
{
	if (vars->parse_tree)
	{
		free_command(vars->parse_tree);
		vars->parse_tree = NULL;
	}
	free_token_list(vars->tokens);
	if (vars->prompt && *vars->prompt)
	{
		free(*vars->prompt);
		*vars->prompt = NULL;
		vars->prompt = NULL;
	}
	if (vars->args)
	{
		free(vars->args);
		vars->args = NULL;
	}
	if (vars->nice_prompt && vars->pwd_malloced)
	{
		free(vars->nice_prompt);
		vars->nice_prompt = NULL;
	}
}

void	free_minishell(t_vars *vars)
{
	free_mini_line(vars);
	free_all_envs();
	rl_clear_history();
}
