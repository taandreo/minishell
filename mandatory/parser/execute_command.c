/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 00:46:26 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/18 00:46:28 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_pipeline(t_pipeline *pipeline, t_vars *vars);
int	execute_conjunctions(t_conjunctions *conj, t_vars *vars);
int	execute_command_part(t_command_part *cmd_part, t_vars *vars);
int	execute_fork_command(t_command_part *data, t_vars *vars);

int	free_cmd(t_command **cmd, t_vars *vars)
{
	free_command(*cmd);
	*cmd = NULL;
	vars->state.is_set = true;
	return (vars->state.status);
}

int	execute_command(t_command **cmd, t_vars *vars)
{
	if (!vars->state.is_set)
	{
		vars->state.status = SUCCESS;
		vars->state.error = false;
	}
	if ((*cmd)->pipeline)
	{
		vars->state.status = execute_pipeline((*cmd)->pipeline, vars);
		if (vars->state.status != SUCCESS)
			return (free_cmd(cmd, vars));
	}
	if ((*cmd)->conjunctions)
	{
		vars->state.status = execute_conjunctions((*cmd)->conjunctions, vars);
		if (vars->state.status != SUCCESS)
			return (free_cmd(cmd, vars));
	}
	return (vars->state.status);
}

int	execute_conjunctions(t_conjunctions *conj, t_vars *vars)
{
	while (conj)
	{
		if (conj->type == TOKEN_AND)
		{
			if (vars->state.status != SUCCESS)
				return (vars->state.status);
			execute_pipeline(conj->pipeline, vars);
		}
		else
		{
			if (vars->state.status == SUCCESS)
				return (vars->state.status);
			execute_pipeline(conj->pipeline, vars);
		}
		conj = conj->next;
	}
	return (vars->state.status);
}
