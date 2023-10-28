/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   restore_fd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 22:32:20 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/28 21:43:02 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restore_stdout(int saved_stdout, t_vars *vars)
{
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
	{
		write(STDERR_FILENO, "minishell: dup2",
			ft_strlen("minishell: dup2"));
		perror("");
		vars->state.error = true;
		vars->state.is_set = true;
		vars->state.status = GENERAL_ERROR;
	}
}

void	restore_stdin(int saved_stdin, t_vars *vars)
{
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
	{
		write(STDERR_FILENO, "minishell: dup2",
			ft_strlen("minishell: dup2"));
		perror("");
		vars->state.error = true;
		vars->state.status = GENERAL_ERROR;
	}
}
