/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 22:07:29 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/28 21:39:40 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	perror("minishell: ");
	free_minishell(vars);
	exit(exit_code);
}
