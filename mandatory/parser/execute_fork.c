/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_fork_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 00:52:03 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/16 02:08:00 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_fork_command(t_command_part *data, t_vars *vars)
{
	if (data->out_pipe[1] != -1)
	{
		close(data->out_pipe[0]);
		dup2(data->out_pipe[1], STDOUT_FILENO);
		vars->changed_stdout = true;
	}
	if (data->in_pipe[0] != -1)
	{
		close(data->in_pipe[1]);
		dup2(data->in_pipe[0], STDIN_FILENO);
		vars->changed_stdin = true;
	}
	data->forked = true;
	return (execute_command_part(data, vars));
}

t_bool	has_heredoc(t_redirections *redir)
{
	t_redirections	*current;

	current = redir;
	while (current)
	{
		if (current->redirection->type == TOKEN_REDIRECTION_HEREDOC)
			return (true);
		current = current->next;
	}
	return (false);
}

void	sync_process(pid_t pid, int sync_pipe[2])
{
	char	wait_signal;

	if (pid == 0)
	{
		trigger_child_sigusr(false);
		close(sync_pipe[0]);
		wait_signal = 'a';
		write(sync_pipe[1], &wait_signal, 1);
		close(sync_pipe[1]);
	}
	else
	{
		close(sync_pipe[1]);
		read(sync_pipe[0], &wait_signal, 1);
		close(sync_pipe[0]);
		kill(pid, SIGUSR1);
	}
}

void	execute_fork(t_pipeline *pipeline, t_vars *vars)
{
	pid_t	pid;
	int		sync_pipe[2];

	if (pipe(sync_pipe) == -1)
		free_and_perror(vars, EXIT_FAILURE);
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
		sync_process(pid, sync_pipe);
		execute_fork_command(pipeline->cmd_part, vars);
	}
	else
	{
		pipeline->cmd_part->pid = pid;
		sync_process(pid, sync_pipe);
	}
}
