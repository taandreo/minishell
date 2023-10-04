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
	int				pipex[2];
	t_command_part	*cmd;

	while (pipeline)
	{
		cmd = pipeline->cmd_part;
		if (pipeline->type == TOKEN_PIPE)
		{
			pipe(pipex);
			dup2(pipex[1], STDOUT_FILENO);
		}
		vars->state.status = execute_command_part(pipeline->cmd_part, vars);
		if (vars->state.status != SUCCESS)
		{
			//close fds
			free_pipeline(pipeline);
			return (vars->state.status);
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
		}
		else
		{
			if (vars->state.status == SUCCESS)
			{
				free_conjunctions(conj);
				return (vars->state.status);
			}
			vars->state.status = execute_pipeline(conj->pipeline, vars);
		}
		conj = conj->next;
	}
	return (vars->state.status);
}

int	output_redirection(t_redirections *files)
{
	int		fd;
	char	*file;

	fd = 1;
	while(files)
	{
		if (files->redirection->type == "TOKEN_REDIRECTION_OUTPUT" 
			|| files->redirection->type == "TOKEN_REDIRECTION_APPEND")
		{
			if (fd != -2)
				close(fd);
			file = files->redirection->filename;
			if (files->redirection->type == "TOKEN_REDIRECTION_OUTPUT")
				fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			if (files->redirection->type == "TOKEN_REDIRECTION_APPEND")
				fd = open(file, O_WRONLY | O_APPEND, 0666);
			if (fd == -1)
				return (-1);
		}
	}
	return (fd);
}

int	execute_command_part(t_command_part *cmd_part, t_vars *vars)
{
	int	input_fd;
	int	output_fd;
	int	fd;

	output_fd = output_redirection();
	output_fd = input_redirection();


}

int	execute_bultin(t_command_part )
{

}