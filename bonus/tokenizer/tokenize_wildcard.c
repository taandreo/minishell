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

int	tokenize_wildcard(char *input, size_t *pos, t_token_list **tokens)
{
	int	exit_status;

	exit_status = add_token(tokens, TOKEN_WILDCARD, "*");
	if (exit_status != SUCCESS)
		return (exit_status);
	(*pos)++;
	if (input[*pos] && ft_is_space(input[*pos]))
		exit_status = add_token(tokens, TOKEN_SPACE, " ");
	return (exit_status);
}

int	add_string_and_maybe_space(char *string, t_token_list **tokens,
		char *input, size_t pos)
{
	int	exit_status;

	if ((*tokens)->tail->token.type == TOKEN_EXIT_CODE)
	{
		exit_status = add_token(tokens, TOKEN_STRING, string);
		if (exit_status != SUCCESS)
			return (exit_status);
		if (input[pos] && input[pos] == ' ' && input[pos + 1])
			exit_status = add_token(tokens, TOKEN_SPACE, " ");
	}
	else
		exit_status = add_token(tokens, TOKEN_STRING, string);
	return (exit_status);
}
