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

int	add_special_or_string(char *string, t_token_list **tokens, char *input,
		size_t pos)
{
	if ((*tokens)->tail->token.type == TOKEN_ECHO
		&& ft_strcmp(string, "-n") == 0)
		return (add_token(tokens, TOKEN_SPECIAL_ARG, string));
	else
		return (add_string_and_maybe_space(string, tokens, input, pos));
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
