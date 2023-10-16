/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 00:23:50 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/16 01:55:22 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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
		return (wait_process(start));
	return (vars->state.status);
}
