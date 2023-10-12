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

void	close_pipe(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

void	copy_pipe(int src[2], int dst[2])
{
	dst[0] = src[0];
	dst[1] = src[1];
}


void	execute_fork(t_pipeline *pipeline, t_vars *vars)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		free_and_perror(vars, EXIT_FAILURE);
	if (pid == 0)
		execute_fork_command(pipeline->cmd_part, vars);
	pipeline->cmd_part->pid = pid;
}

int	execute_pipeline(t_pipeline *pipeline, t_vars *vars)
{
	t_pipeline		*start;

	start = pipeline;
	if (is_builtin_token(pipeline->type) && pipeline->type == TOKEN_NONE)
		return(execute_command_part(pipeline->cmd_part, vars));
	while(pipeline->type == TOKEN_PIPE)
	{ 
		pipe(pipeline->cmd_part->out_pipe);
		copy_pipe(pipeline->cmd_part->out_pipe, pipeline->next->cmd_part->in_pipe);
		execute_fork(pipeline, vars);
		close_pipe(pipeline->cmd_part->out_pipe);
		pipeline = pipeline->next;
	}	
	execute_fork(pipeline, vars);
	execute_fork_command(pipeline->cmd_part, vars);
	return(wait_process(start));
}


// int	output_redirection(t_redirections *files)
// {
// 	int		fd;
// 	char	*file;

// 	fd = 1;
// 	while(files)
// 	{
// 		if (files->redirection->type == "TOKEN_REDIRECTION_OUTPUT" 
// 			|| files->redirection->type == "TOKEN_REDIRECTION_APPEND")
// 		{
// 			if (fd != -2)
// 				close(fd);
// 			file = files->redirection->filename;
// 			if (files->redirection->type == "TOKEN_REDIRECTION_OUTPUT")
// 				fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
// 			if (files->redirection->type == "TOKEN_REDIRECTION_APPEND")
// 				fd = open(file, O_WRONLY | O_APPEND, 0666);
// 			if (fd == -1)
// 				return (-1);
// 		}
// 	}
// 	return (fd);
// }

// int	set_redirections(t_redirections *files)
// {
// 	int	out_fd;

// 	out_fd = output_redirection(files)
// 	if (out_fd == -1)
// 	{
// 		;
// 	}
// 	in_fd  = input_redirection(files)
// }

char	**list_to_args(t_arguments *list)
{
	size_t		len;
	t_arguments	*start;
	char		**args;
	size_t		i;

	if (list == NULL)
		return (NULL);
	start = list;
	len = 0;
	while (list)
	{
		len++;
		list = list->next;
	}
	args = ft_calloc(len + 1, sizeof(char *));
	i = 0;
	while (start)
	{
		args[i] = start->string;
		i++;
	}
	args[i] = NULL;
}

int	execute_bultin(t_command_part *data, t_vars *vars)
{
	if (data->type == TOKEN_PWD)
		return(builtin_pwd(data->args));
	if (data->type == TOKEN_CD)
		return(builtin_cd(data->args));
	if (data->type == TOKEN_ECHO)
		return(builtin_echo(data->args));
	if (data->type == TOKEN_ENV)
		return(builtin_env(data->args));
	if (data->type == TOKEN_EXPORT)
		return(builtin_export(data->args));
	if (data->type == TOKEN_UNSET)
		return(builtin_unset(data->args));
	// adicionar o status do ultimo comando
	if (data->type == TOKEN_EXIT)
		return(builtin_exit(data->args, vars));
	return (0);
}

int	execute_fork_command(t_command_part *data, t_vars *vars)
{
	char **args;

	if (data->out_pipe[1] != -1)
	{
		// if there is stdout pipe, closes the read side of this pipe
		close(data->out_pipe[0]);
		dup2(STDOUT_FILENO, data->out_pipe[1]);
	}
	if (data->in_pipe[0] != -1)
	{
		// if there is a stdin pipe, closes the write side of this pipe
		close(data->in_pipe[1]);
		dup2(STDIN_FILENO, data->in_pipe[0]);
	}
	data->forked = true;
	execute_command_part(data, vars);
}

int	execute_command_part(t_command_part *data, t_vars *vars)
{
	size_t	exit_code;
	int		infile_fd;

	if (update_cmd_part_values(data, vars) != SUCCESS)
		return (vars->state.status);
	output_redirection(data->redirections)
	infile_fd = data->args = list_to_args(data->arguments);
	if ()

	if (data->type == TOKEN_COMMAND_NAME)
	{
		data->cmd_path = cmd_path_routine(data->u_cmd.cmd_name->value, vars);
		if (!data->cmd_path)
		{
			free_minishell(vars);
			exit(vars->state.status);
		}
		execve(data->cmd_path, data->args, g_env);
		handle_exec_errors(data->cmd_path, data->args, data);
	} 
	else if (is_builtin_token(data->type) && data->forked)
	{
		exit_code = execute_builtin(data, vars);
		free_minishell(vars);
		exit(exit_code);
	}
	else if (is_builtin_token(data->type) && !data->forked)
		execute_builtin(data, vars);
}
