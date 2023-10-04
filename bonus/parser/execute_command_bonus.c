#include "minishell_bonus.h"

int	execute_pipeline(t_pipeline *pipeline, t_vars *vars);
int	execute_conjunctions(t_conjunctions *conj, t_vars *vars);
int	execute_command_part(t_command_part *cmd_part, t_vars *vars);

int	execute_command(t_command **cmd, t_vars *vars)
{
	vars->state.status = SUCCESS;
	vars->state.error = false;
	if ((*cmd)->pipeline)
	{
		vars->state.status = execute_pipeline((*cmd)->pipeline, vars);
		if (vars->state.status != SUCCESS)
		{
			free_command(*cmd);
			*cmd = NULL;
			return (vars->state.status);
		}
	}
	if ((*cmd)->conjunctions)
	{
		vars->state.status = execute_conjunctions((*cmd)->conjunctions, vars);
		if (vars->state.status != SUCCESS)
		{
			free_command(*cmd);
			*cmd = NULL;
			return (vars->state.status);
		}
	}
	return (vars->state.status);
}

int	execute_pipeline(t_pipeline *pipeline, t_vars *vars)
{
	// create pipelinefd[2]
//	int fd[2] = pipe()
	while (pipeline)
	{
		if (pipeline->type == TOKEN_PIPE)
		{
			//dup2 stdout to write and fork
//			dup2()

		}
		vars->state.status = execute_command_part(pipeline->cmd_part, vars);
		if (vars->state.status != SUCCESS)
			return (vars->state.status);
		if (pipeline->type == TOKEN_PIPE)
		{
			//close write
//			close(fd[1])
//			dup2(fd[0], STDIN)
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
				return (vars->state.status);
			vars->state.status = execute_pipeline(conj->pipeline, vars);
		}
		else
		{
			if (vars->state.status == SUCCESS)
				return (vars->state.status);
			vars->state.status = execute_pipeline(conj->pipeline, vars);
		}
		conj = conj->next;
	}
	return (vars->state.status);
}

int	execute_command_part(t_command_part *cmd_part, t_vars *vars)
{
	if (update_cmd_part_values(cmd_part, vars) != SUCCESS)
		return (vars->state.status);
	if (is_builtin_token(cmd_part->type))
		vars->state.status = execute_builtin(cmd_part->u_cmd.builtin_cmd->value,
				cmd_part, vars);
	else if (cmd_part->type == TOKEN_COMMAND_NAME)
		vars->state.status = execute_cmd_name(cmd_part->u_cmd.cmd_name->value,
				cmd_part, vars);
	else
		vars->state.status = execute_redirections_only(cmd_part->redirections,
				vars);
	return (vars->state.status);
}

