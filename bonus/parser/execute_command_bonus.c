#include "minishell_bonus.h"

int	execute_pipeline(t_pipeline *pipeline, t_vars *vars);
int	execute_conjunctions(t_conjunctions *conj, t_vars *vars);
int	execute_command_part(t_command_part *cmd_part, t_vars *vars);
int	execute_fork_command(t_command_part *data, t_vars *vars);

int	execute_command(t_command **cmd, t_vars *vars)
{
	if (!vars->state.is_set)
	{
		vars->state.status = SUCCESS;
		vars->state.error = false;
	}
	if ((*cmd)->pipeline)
	{
		execute_pipeline((*cmd)->pipeline, vars);
		if (vars->state.status != SUCCESS)
		{
			free_command(*cmd);
			*cmd = NULL;
			vars->state.is_set = true;
			return (vars->state.status);
		}
	}
	if ((*cmd)->conjunctions)
	{
		execute_conjunctions((*cmd)->conjunctions, vars);
		if (vars->state.status != SUCCESS)
		{
			free_command(*cmd);
			*cmd = NULL;
			vars->state.is_set = true;
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

t_bool has_heredoc(t_redirections *redir)
{
	t_redirections *current;

	current = redir;
	while (current)
	{
		if (current->redirection->type == TOKEN_REDIRECTION_HEREDOC)
			return (true);
		current = current->next;
	}
	return (false);
}



void	execute_fork(t_pipeline *pipeline, t_vars *vars)
{
	pid_t	pid;
	int		sync_pipe[2];
	char	wait_signal;

	if (pipe(sync_pipe) == -1)
	{
		perror("minishell: ");
		exit(EXIT_FAILURE);
	}
	if (has_heredoc(pipeline->cmd_part->redirections))
		open_heredoc(pipeline->cmd_part->redirections, vars);
	if (vars->close_heredoc)
		return ;
	vars->is_forked = true;
	pid = fork();
	if (pid == -1)
		free_and_perror(vars, EXIT_FAILURE);
	start_signal_forked(pid);
	if (pid == 0)
	{
		trigger_child_sigusr(false);
		close(sync_pipe[0]);
		wait_signal = 'a';
		write(sync_pipe[1], &wait_signal, 1);
		close(sync_pipe[1]);
		execute_fork_command(pipeline->cmd_part, vars);
	}
	else
	{
		pipeline->cmd_part->pid = pid;
		close(sync_pipe[1]);
		read(sync_pipe[0], &wait_signal, 1);
		close(sync_pipe[0]);
		kill(pid, SIGUSR1);
	}
}

int	execute_pipeline(t_pipeline *pipeline, t_vars *vars)
{
	t_pipeline		*start;
	t_pipeline		*prev;

	start = pipeline;
	prev = pipeline;
	if (is_builtin_token(pipeline->cmd_part->type) && pipeline->type == TOKEN_NONE)
		return(execute_command_part(pipeline->cmd_part, vars));
	while(pipeline->type == TOKEN_PIPE)
	{ 
		pipe(pipeline->cmd_part->out_pipe);
		copy_pipe(pipeline->cmd_part->out_pipe, pipeline->next->cmd_part->in_pipe);
		execute_fork(pipeline, vars);
		if (vars->close_heredoc)
			break ;
		if (pipeline != prev)
			close_pipe(prev->cmd_part->out_pipe);
		prev = pipeline;
		pipeline = pipeline->next;
	}
	execute_fork(pipeline, vars);
	if (vars->close_heredoc)
		return (vars->state.status);
	if (pipeline != prev)
		close_pipe(prev->cmd_part->out_pipe);
	if (pipeline->cmd_part->pid != 0 && pipeline->cmd_part->pid != -1)
		return(wait_process(start));
	return (vars->state.status);
}

char	**list_to_args(t_arguments *list, t_command_part *cmd_part)
{
	size_t		len;
	t_arguments	*start;
	char		**args;
	size_t		i;

	start = list;
	len = 0;
	while (list)
	{
		len++;
		list = list->next;
	}
	args = ft_calloc(len + 2, sizeof(char *));
	if (is_token_cmd_name(cmd_part->type))
		args[0] = cmd_part->u_cmd.cmd_name->value;
	else if (is_builtin_token(cmd_part->type))
		args[0] = cmd_part->u_cmd.builtin_cmd->value;
	i = 1;
	while (start)
	{
		args[i] = start->string->value;
		i++;
		start = start->next;
	}
	args[i] = NULL;
	return (args);
}

int	execute_builtin(t_command_part *data, char **args, t_vars *vars)
{
	args++;
	if (data->type == TOKEN_PWD)
		return(builtin_pwd(args));
	if (data->type == TOKEN_CD)
		return(builtin_cd(args));
	if (data->type == TOKEN_ECHO)
		return(builtin_echo(args));
	if (data->type == TOKEN_ENV)
		return(builtin_env(args));
	if (data->type == TOKEN_EXPORT)
		return(builtin_export(args));
	if (data->type == TOKEN_UNSET)
		return(builtin_unset(args));
	if (data->type == TOKEN_EXIT)
		return(builtin_exit(args, vars));
	return (0);
}

int	execute_fork_command(t_command_part *data, t_vars *vars)
{
	if (data->out_pipe[1] != -1)
	{
		// if there is stdout pipe, closes the read side of this pipe
		close(data->out_pipe[0]);
		dup2(data->out_pipe[1], STDOUT_FILENO);
		vars->changed_stdout = true;
	}
	if (data->in_pipe[0] != -1)
	{
		// if there is a stdin pipe, closes the write side of this pipe
		close(data->in_pipe[1]);
		dup2(data->in_pipe[0], STDIN_FILENO);
		vars->changed_stdin = true;
	}
	data->forked = true;
	return (execute_command_part(data, vars));
}

size_t get_env_size()
{
	size_t	i;
	t_list	*current;

	i = 0;
	current = g_vars.env;
	while (current)
	{
		i++;
		current = current->next;
	}
	return (i);
}

char *join_tmp_key_value(t_env *tmp)
{
	char *tmp2;
	char *envp;

	tmp2 = ft_strjoin(tmp->key, "=");
	if (!tmp2)
		return (return_mem_alloc_error());
	envp = ft_strjoin(tmp2, tmp->value);
	if (!envp)
		return (return_mem_alloc_error());
	free(tmp2);
	return (envp);
}


char **list_to_envp()
{
	size_t	len;
	char	**envp;
	t_list	*current;
	t_env	*tmp;
	size_t	i;

	i = 0;
	current = g_vars.env;
	len = get_env_size();
	envp = ft_calloc((len + 1),sizeof(char *));
	while (current)
	{
		tmp = current->content;
		envp[i] = join_tmp_key_value(tmp);
		if (!envp[i])
			return (NULL);
		current = current->next;
		i++;
	}
	return (envp);
}

void	handle_exec_errors(t_vars *vars, char **envp)
{
	free_minishell(vars);
	if (envp)
	{
		ft_freemt((void **)envp);
		envp = NULL;
	}
	exit(vars->state.status);
}

int	execute_command_part(t_command_part *data, t_vars *vars)
{
	int			exit_code;
	char			**envp;

	if (update_cmd_part_values(data, vars) != SUCCESS)
		return (vars->state.status);
	data->args = list_to_args(data->arguments, data);
	execute_redirections(data, vars);
	if (vars->state.error == true)
	{
		if (data->forked == true)
		{
			free_minishell(vars);
			exit(vars->state.status);
		}
		return (vars->state.status);
	}
	if (is_token_cmd_name(data->type))
	{
		data->cmd_path = cmd_path_routine(data->u_cmd.cmd_name->value,
				data->type, vars);
		if (!data->cmd_path)
		{
			free_minishell(vars);
			exit(vars->state.status);
		}
		envp = list_to_envp();
		execve(data->cmd_path, data->args, envp);
		handle_exec_errors(vars, envp);
	}
	else if (data->type == TOKEN_GROUP)
		execute_command(&data->u_cmd.grouping->enclosed_cmd, vars);
	else if (is_builtin_token(data->type) && data->forked)
	{
		exit_code = execute_builtin(data, data->args, vars);
		free_minishell(vars);
		exit(exit_code);
	}
	else if (is_builtin_token(data->type) && !data->forked)
	{
		exit_code = execute_builtin(data, data->args, vars);
		if (vars->changed_stdin)
		{
			restore_stdin(vars->saved_stdin, vars);
			vars->changed_stdin = false;
		}
		if (vars->changed_stdout)
		{
			restore_stdout(vars->saved_stdout, vars);
			vars->changed_stdout = false;
		}
//		trigger_child_sigusr(false);
		return (exit_code);
	}
	return (vars->state.status);
}
