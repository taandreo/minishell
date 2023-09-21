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

#include "minishell_bonus.h"

int	tokenize_wildcard(char *input, size_t *pos, t_token_list **tokens,
		t_token_flags *flags)
{
	int	exit_status;

	exit_status = add_token(tokens, TOKEN_WILDCARD, "*");
	if (exit_status != SUCCESS)
		return (exit_status);
	(*pos)++;
	if (flags->is_command)
	{
		if (!input[*pos] || input[*pos] == ' ')
		{
			flags->is_command = false;
			flags->has_command = true;
		}
	}
	else if (flags->is_redirection)
	{
		if (!input[*pos] || input[*pos] == ' ')
		{
			flags->is_redirection = false;
			if (!flags->has_command)
				flags->is_command = true;
		}
	}
	return (exit_status);
}
