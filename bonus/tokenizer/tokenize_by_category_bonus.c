/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_by_category_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:31:47 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:31:49 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	tokenize_by_category(char **input, size_t *position,
		t_token_list **tokens, t_token_flags *flags)
{
	char	c;

	c = (*input)[*position];
	flags->has_exit_code = false;
	if (has_quotes(c))
		return (tokenize_quotes(input, position, tokens, flags));
	else if (c == '<' || c == '>')
		return (tokenize_redirections(*input, position, tokens, flags));
	else if (c == '&' || c == '|')
		return (tokenize_operators(*input, position, tokens, flags));
	else if (c == '(' || c == ')')
		return (tokenize_parenthesis(*input, position, tokens, flags));
	return (tokenize_strings(input, position, tokens, flags));
}
