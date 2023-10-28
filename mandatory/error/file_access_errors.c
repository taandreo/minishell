/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_access_errors.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:33:33 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/28 21:39:47 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*return_is_dir_error_cmd(char *str, t_vars *vars)
{
	write(STDERR_FILENO, "minishell: ",
		ft_strlen("minishell: "));
	write(STDERR_FILENO, str,
		ft_strlen(str));
	write(STDERR_FILENO, ": Is a directory\n",
		ft_strlen(": Is a directory\n"));
	vars->state.status = NOTEXEC;
	return (NULL);
}

void	*return_no_such_cmd(char *str, t_vars *vars)
{
	write(STDERR_FILENO, "minishell: ",
		ft_strlen("minishell: "));
	write(STDERR_FILENO, str,
		ft_strlen(str));
	write(STDERR_FILENO, ": No such file or directory\n",
		ft_strlen(": No such file or directory\n"));
	vars->state.status = CMDNFND;
	return (NULL);
}

void	*return_permission_denied(char *str, t_vars *vars)
{
	write(STDERR_FILENO, "minishell: ",
		ft_strlen("minishell: "));
	write(STDERR_FILENO, str,
		ft_strlen(str));
	write(STDERR_FILENO, ": Permission denied\n",
		ft_strlen(": Permission denied\n"));
	vars->state.status = NOTEXEC;
	return (NULL);
}

void	*return_cmd_not_found(char *str, t_vars *vars)
{
	write(STDERR_FILENO, "minishell: ",
		ft_strlen("minishell: "));
	write(STDERR_FILENO, str,
		ft_strlen(str));
	write(STDERR_FILENO, ": command not found\n",
		ft_strlen(": command not found\n"));
	vars->state.status = CMDNFND;
	return (NULL);
}
