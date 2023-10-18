/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils2_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:30:44 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:32:30 by ebezerra         ###   ########.fr       */
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
