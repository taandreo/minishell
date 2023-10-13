/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 22:07:29 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/12 19:35:46 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	print_perror(char *cmd, char *msg)
{
	if (cmd != NULL)
		ft_dprintf(STDERR_FILENO, "minishell: %s: ", cmd);
	else
		ft_dprintf(STDERR_FILENO, "minishell: ");
	perror(msg);
}

void	free_and_perror(t_vars *vars, int exit_code)
{
	perror("minishell");
	free_minishell(vars);
	exit(exit_code);
}

void	free_and_exit(t_vars *vars, int exit_code)
{
	free_minishell(vars);
	exit(exit_code);
}