/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_errors_helper_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:33:04 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/15 22:29:51 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_mem_alloc_error(void)
{
	write(STDERR_FILENO, "Error: Memory allocation failed\n",
		ft_strlen("Error: Memory allocation failed\n"));
}

void	mem_alloc_set_error(t_vars *vars)
{
	print_mem_alloc_error();
	vars->state.status = MISUSE;
	vars->state.error = true;
	vars->state.is_set = true;
}

void	error_open_file(char *file, t_vars *vars)
{
	write(STDERR_FILENO, "minishell: ",
		ft_strlen("minishell: "));
	write(STDERR_FILENO, file,
		ft_strlen(file));
	write(STDERR_FILENO, ": ", ft_strlen(": "));
	perror("");
	vars->state.error = true;
	vars->state.is_set = true;
	vars->state.status = GENERAL_ERROR;
}

void	*return_mem_alloc_error(void)
{
	print_mem_alloc_error();
	return (NULL);
}

void	*return_syntax_error(const char *value)
{
	write(STDERR_FILENO, "minishell: syntax error near unexpected token `",
		ft_strlen("minishell: syntax error near unexpected token `"));
	write(STDERR_FILENO, value, ft_strlen(value));
	write(STDERR_FILENO, "'\n", ft_strlen("'\n"));
	return (NULL);
}
