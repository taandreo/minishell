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

void	close_pipe(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

// int	**allocate_fd(int size)
// {

// 	fd = ft_calloc(size - 1, sizeof (*fd));
// 	if (!fd)
// 	{
// 		perror("bash");
// 		exit(EXIT_FAILURE);
// 	}
// 	return (fd_alloc_routine(fd, data->cmd_num));
// }

// int	*create_pipes(int size)
// {
// 	int	i;
// 	int *pipes;

// 	i = 0;
// 	pipes = ft_calloc((size - 1) * 2, sizeof(int));
// 	while (i < size - 1)
// 	{
// 		if (pipe(pipes == -1)
// 		{
// 			perror("bash");
// 			free_vars(data);
// 			exit(EXIT_FAILURE);
// 		}
// 		i + 2;
// 	}
// 	return (data->fd);
// }

static void	child_proc_routine(char **argv, char **environ, t_command_part *data, int i)
{
	if (i == 0)
		first_cmd_routine(data, argv, i);
	if (i == data->cmd_num - 1)
		last_cmd_routine(data, argv, i);
	if (i > 0 && i < data->cmd_num - 1)
		intermediates_cmd_routine(data, i);
	data->bin_file = get_bin_name(argv[i + data->inc]);
	data->cmd_path = cmd_path_routine(data->bin_file, data);
	check_cmd_not_found(data->cmd_path, data, data->bin_file);
	if (check_for_quotes(argv[i + data->inc]))
	{
		data->cmd_args = split_with_quotes(argv[i + data->inc]);
		if (!data->cmd_args)
			invalid_num_quotes(data);
	}
	else
		data->cmd_args = ft_split(argv[i + data->inc], ' ');
	free(data->bin_file);
	execve(data->cmd_path, data->cmd_args, environ);
	handle_exec_errors(data->cmd_path, data->cmd_args, data);
}

int	get_pipe_cmd(t_pipeline *pipeline)
{
	int i;

	i = 0;
	while(pipeline && pipeline->type == TOKEN_PIPE)
		i++;
	return (i + 1);
}

void	copy_pipe(int src[2], int dst[2])
{
	dst[0] = src[0];
	dst[1] = src[1];
}

void	handle_pipe(t_pipeline **pipeline, t_vars *vars)
{
	t_pipeline	*p;

	p = *pipeline;
	while(p->type == TOKEN_PIPE)
	{ 
		pipe(p->cmd_part->out_pipe);
		copy_pipe(p->cmd_part->out_pipe, p->next->cmd_part->in_pipe);
		execute_command_part(p->cmd_part, vars);
		close_pipe(p->cmd_part->out_pipe);
	}
	wait_forks()
	execute_command_part(p->cmd_part, vars);
}

int	execute_pipeline(t_pipeline *pipeline, t_vars *vars)
{
	int				pipex[2];
	t_command_part	*cmd;
	size_t			i;

	while(pipeline->type == TOKEN_PIPE)
	{ 
		pipe(pipeline->cmd_part->out_pipe);
		copy_pipe(pipeline->cmd_part->out_pipe, pipeline->next->cmd_part->in_pipe);
		execute_command_part(pipeline->cmd_part, vars);
		close_pipe(pipeline->cmd_part->out_pipe);
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

static void	child_proc_routine(t_command_part *data, t_vars *vars)
{
	if (data->out_pipe[1] != -1)
		// if there is stdout pipe, closes the read side of this pipe
		close(data->out_pipe[0]);
		dup2(STDOUT_FILENO, fd[1]);
	if (data->in_pipe[0] != -1)
		// if there is a stdin pipe, closes the write side of this pipe
		close(data->r_pipe[1]);
		dup2(STDIN_FILENO, fd[1]);
		// last_cmd_routine(data, argv, i);
	data->bin_file = get_bin_name(argv[i + data->inc]);
	data->cmd_path = cmd_path_routine(data->bin_file, data);
	check_cmd_not_found(data->cmd_path, data, data->bin_file);
	if (check_for_quotes(argv[i + data->inc]))
	{
		data->cmd_args = split_with_quotes(argv[i + data->inc]);
		if (!data->cmd_args)
			invalid_num_quotes(data);
	}
	else
		data->cmd_args = ft_split(argv[i + data->inc], ' ');
	free(data->bin_file);
	execve(data->cmd_path, data->cmd_args, environ);
	handle_exec_errors(data->cmd_path, data->cmd_args, data);
}

int	execute_command_part(t_command_part *cmd_part, t_vars *vars)
{
	int	input_fd;
	int	output_fd;
	int	fd;

	if (update_cmd_part_values(cmd_part, vars) != SUCCESS)
		return (vars->state.status);
	
	// output_fd = output_redirection();
	// output_fd = input_redirection();

	if ()
}