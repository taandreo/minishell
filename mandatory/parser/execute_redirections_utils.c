/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirections_utils_bonus.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 00:19:01 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/16 01:17:23 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	input_file_to_stdin(int infile, t_vars *vars)
{
	if (dup2(infile, STDIN_FILENO) == -1)
	{
		write(STDERR_FILENO, "minishell: dup2", ft_strlen("minishell: dup2"));
		perror("");
		vars->state.error = true;
		vars->state.is_set = true;
		vars->state.status = GENERAL_ERROR;
	}
	vars->changed_stdin = true;
}

void	output_file_to_stdout(int outfile, t_vars *vars)
{
	if (dup2(outfile, STDOUT_FILENO) == -1)
	{
		write(STDERR_FILENO, "minishell: dup2", ft_strlen("minishell: dup2"));
		perror("");
		vars->state.error = true;
		vars->state.is_set = true;
		vars->state.status = GENERAL_ERROR;
	}
	vars->changed_stdout = true;
}
