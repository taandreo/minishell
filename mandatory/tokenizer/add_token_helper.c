/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_token_helper_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:30:51 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:31:24 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	add_token_1_pos(size_t *pos, t_token_list **tokens,
		t_token_type type, t_token_flags *flags)
{
	(*pos)++;
	if (type == TOKEN_REDIRECTION_INPUT || type == TOKEN_REDIRECTION_OUTPUT)
	{
		flags->has_heredoc = false;
		if (type == TOKEN_REDIRECTION_INPUT)
			return (add_token(tokens, type, "<"));
		return (add_token(tokens, type, ">"));
	}
	if (type == TOKEN_PIPE)
		return (add_token(tokens, type, "|"));
	if (type == TOKEN_LEFT_PARENTHESIS)
		return (add_token(tokens, type, "("));
	if (type == TOKEN_RIGHT_PARENTHESIS)
		return (add_token(tokens, type, ")"));
	return (add_token(tokens, type, "&"));
}

int	add_token_2_pos(size_t *pos, t_token_list **tokens,
		t_token_type type, t_token_flags *flags)
{
	*pos += 2;
	if (type == TOKEN_REDIRECTION_HEREDOC)
	{
		flags->has_heredoc = true;
		return (add_token(tokens, type, "<<"));
	}
	if (type == TOKEN_REDIRECTION_APPEND)
	{
		flags->has_heredoc = false;
		return (add_token(tokens, type, ">>"));
	}
	if (type == TOKEN_AND)
		return (add_token(tokens, type, "&&"));
	return (add_token(tokens, type, "||"));
}
