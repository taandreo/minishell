/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 00:23:50 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/28 21:42:45 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_bool	pipe_routine(t_pipeline *pipeline, t_pipeline *prev, t_vars *vars)
{
	if (pipe(pipeline->cmd_part->out_pipe) == -1)
	{
		perror("minishell: pipe:");
		vars->state.error = true;
		vars->state.status = GENERAL_ERROR;
		return (false);
	}
	copy_pipe(pipeline->cmd_part->out_pipe, pipeline->next->cmd_part->in_pipe);
	execute_fork(pipeline, vars);
	if (pipeline != prev)
		close_pipe(prev->cmd_part->out_pipe);
	return (true);
}

int	execute_pipeline(t_pipeline *pipeline, t_vars *vars)
{
	t_pipeline		*start;
	t_pipeline		*prev;

	start = pipeline;
	prev = pipeline;
	if (is_builtin_token(pipeline->cmd_part->type)
		&& pipeline->type == TOKEN_NONE)
		return (execute_command_part(pipeline->cmd_part, vars));
	while (pipeline->type == TOKEN_PIPE)
	{
		if (!pipe_routine(pipeline, prev, vars))
			return (vars->state.status);
		if (vars->close_heredoc)
			return (vars->state.status);
		prev = pipeline;
		pipeline = pipeline->next;
	}
	execute_fork(pipeline, vars);
	if (vars->close_heredoc)
		return (vars->state.status);
	if (pipeline != prev)
		close_pipe(prev->cmd_part->out_pipe);
	if (pipeline->cmd_part->pid != 0 && pipeline->cmd_part->pid != -1)
		return (wait_process(start));
	return (vars->state.status);
}
