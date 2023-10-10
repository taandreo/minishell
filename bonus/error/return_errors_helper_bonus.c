/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_errors_helper_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:33:04 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/19 22:20:24 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	print_mem_alloc_error(void)
{
	write(STDERR_FILENO, "Error: Memory allocation failed\n",
		ft_strlen("Error: Memory allocation failed\n"));
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


