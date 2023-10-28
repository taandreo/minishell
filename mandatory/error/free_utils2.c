/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:30:44 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/28 21:39:52 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_2_str_and_nullify(char **str1, char **str2)
{
	free_str_and_nullify(str1);
	free_str_and_nullify(str2);
}

void	*free_str_nullify_and_malloc_error(char **str)
{
	free_str_and_nullify(str);
	return (return_mem_alloc_error());
}
