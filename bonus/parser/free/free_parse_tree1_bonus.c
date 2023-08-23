#include "minishell_bonus.h"

void	free_command_part(t_command_part *cmd_part)
{
	if (cmd_part)
	{
		if (cmd_part->type == TOKEN_COMMAND_NAME)
			free(cmd_part->u_cmd.cmd_name);
		else if (cmd_part->type >= TOKEN_ECHO && cmd_part->type <= TOKEN_EXIT)
			free_builtin_command(cmd_part->u_cmd.builtin_cmd);
		free_arguments(cmd_part->arguments);
		free_redirections(cmd_part->redirections);
		free(cmd_part);
	}
}

void	free_pipeline(t_pipeline *pipe)
{
	if (pipe)
	{
		free_command_part(pipe->cmd_part);
		free_pipeline(pipe->next);
		free(pipe);
	}
}

void	free_conjunction(t_conjunctions *conj)
{
	if (conj)
	{
		free_pipeline(conj->pipeline);
		free_conjunction(conj->next);
		free(conj);
	}
}

void	free_command(t_command *cmd)
{
	if (cmd)
	{
		free_pipeline(cmd->pipeline);
		free_conjunction(cmd->conjunctions);
		free(cmd);
	}
}
