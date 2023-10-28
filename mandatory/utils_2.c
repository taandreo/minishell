/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 21:03:47 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/28 21:44:18 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	success_or_mem_error(int exit_status)
{
	if (exit_status == MISUSE)
		print_mem_alloc_error();
	return (exit_status);
}

void	*error_and_exit_code_false(t_token_flags *flags)
{
	flags->has_exit_code = false;
	return (return_mem_alloc_error());
}

void	*null_exit_code_false(t_token_flags *flags)
{
	flags->has_exit_code = false;
	return (NULL);
}

void	*null_exit_code_false_free_string(t_token_flags *flags)
{
	flags->has_exit_code = false;
	return (free_and_return_null(flags->string));
}
