#include "minishell_bonus.h"

int	execute_pipeline(t_pipeline *pipeline, t_vars *vars);
int	execute_conjunctions(t_conjunctions *conj, t_vars *vars);
int	execute_command_part(t_command_part *cmd_part, t_vars *vars);

int	execute_command(t_command *cmd, t_vars *vars)
{
	vars->state.status = SUCCESS;
	vars->state.error = false;
	if (cmd->pipeline)
	{
		vars->state.status = execute_pipeline(cmd->pipeline, vars);
		if (vars->state.status != SUCCESS)
		{
			free_command(cmd);
			return (vars->state.status);
		}
	}
	if (cmd->conjunctions)
	{
		vars->state.status = execute_conjunctions(cmd->conjunctions, vars);
		if (vars->state.status != SUCCESS)
		{
			free_command(cmd);
			return (vars->state.status);
		}
	}
	return (vars->state.status);
}

int	execute_pipeline(t_pipeline *pipeline, t_vars *vars)
{
	// create pipelinefd[2]
	while (pipeline)
	{
		if (pipeline->type == TOKEN_PIPE)
		{
			//dup2 stdout to write and fork
		}
		vars->state.status = execute_command_part(pipeline->cmd_part, vars);
		if (vars->state.status != SUCCESS)
		{
			//close fds
			free_pipeline(pipeline);
			return (vars->state.status);
		}
		if (pipeline->type == TOKEN_PIPE)
		{
			//close write
			//dup2 stdin with read
		}
		pipeline = pipeline->next;
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
			{
				free_conjunctions(conj);
				return (vars->state.status);
			}
			vars->state.status = execute_pipeline(conj->pipeline, vars);
			if (vars->state.status != SUCCESS)
			{
				free_conjunctions(conj);
				return (vars->state.status);
			}
		}
		else
			vars->state.status = execute_pipeline(conj->pipeline, vars);
		conj = conj->next;
	}
	return (vars->state.status);
}

int	execute_command_part(t_command_part *cmd_part, t_vars *vars)
{
	(void)cmd_part;
	return (vars->state.status);
}