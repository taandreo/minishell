/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_wildcard.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:32:04 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:32:05 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tokenize_wildcard(char *input, size_t *pos, t_token_list **tokens,
		t_token_flags *flags)
{
	int	exit_status;

	exit_status = SUCCESS;
	if (input[*pos + 1] != '*')
	{
		exit_status = add_token(tokens, TOKEN_WILDCARD, "*");
		if (exit_status != SUCCESS)
		{
			print_mem_alloc_error();
			return (exit_status);
		}
		set_flags_variables(input, pos, flags);
	}
	(*pos)++;
	return (exit_status);
}
