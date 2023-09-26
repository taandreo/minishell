/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:31:53 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:31:54 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	tokenize_quotes(char **input, size_t *pos,
		t_token_list **tokens, t_token_flags *flags)
{
	int				exit_status;

	exit_status = SUCCESS;
	if (!tokens || !*tokens)
		return (MISUSE);
	flags->string = handle_quotes(input, pos, tokens, flags);
	if (!flags->string && !flags->has_exit_code)
	{
		free_str_and_nullify(&flags->var);
		return (misuse_or_unclosed_quotes_error(tokens));
	}
	if (flags->is_command)
		exit_status = add_builtin_or_command(flags->string, tokens, flags,
				(*input)[*pos]);
	else if (flags->is_redirection)
		exit_status = add_filename_or_string(flags->string, tokens, flags,
				(*input)[*pos]);
	else
	{
		if (flags->string)
			exit_status = add_token(tokens, TOKEN_STRING, flags->string);
	}
	free_2_str_and_nullify(&flags->var, &flags->string);
	return (success_or_mem_error(exit_status));
}

int	tokenize_redirections(char *input, size_t *pos,
		t_token_list **tokens, t_token_flags *flags)
{
	int	status;

	flags->is_command = false;
	flags->is_redirection = true;
	if (!tokens || !*tokens)
		return (MISUSE);
	if (input[*pos] == '<' && peek_next(input, *pos, flags->input_len) == '<')
		status = add_token_2_pos(pos, tokens, TOKEN_REDIRECTION_HEREDOC, flags);
	else if (input[*pos] == '<')
		status = add_token_1_pos(pos, tokens, TOKEN_REDIRECTION_INPUT, flags);
	else if (input[*pos] == '>'
		&& peek_next(input, *pos, flags->input_len) == '>')
		status = add_token_2_pos(pos, tokens, TOKEN_REDIRECTION_APPEND, flags);
	else
		status = add_token_1_pos(pos, tokens, TOKEN_REDIRECTION_OUTPUT, flags);
	return (success_or_mem_error(status));
}

int	tokenize_operators(char *input, size_t *pos,
		t_token_list **tokens, t_token_flags *flags)
{
	int	status;

	flags->is_command = true;
	flags->is_redirection = false;
	flags->has_command = false;
	if (!tokens || !*tokens)
		return (MISUSE);
	if (input[*pos] == '|' && peek_next(input, *pos, flags->input_len) == '|')
		status = add_token_2_pos(pos, tokens, TOKEN_OR, flags);
	else if (input[*pos] == '|')
		status = add_token_1_pos(pos, tokens, TOKEN_PIPE, flags);
	else if (input[*pos] == '&' && peek_next(
			input, *pos, flags->input_len) == '&')
		status = add_token_2_pos(pos, tokens, TOKEN_AND, flags);
	else
		status = add_token_1_pos(pos, tokens, TOKEN_INVALID, flags);
	return (success_or_mem_error(status));
}

int	tokenize_strings(char **input, size_t *pos,
		t_token_list **tokens, t_token_flags *flags)
{
	char		*return_string;
	int			exit_status;

	exit_status = SUCCESS;
	if (!tokens || !*tokens)
		return (MISUSE);
	return_string = get_string_from_input(input, pos, tokens, flags);
	if (!return_string && !flags->has_exit_code)
		return (misuse_or_unclosed_quotes_error(tokens));
	if (flags->is_command)
		exit_status = add_builtin_or_command(return_string, tokens, flags,
				(*input)[*pos]);
	else if (flags->is_redirection)
		exit_status = add_filename_or_string(return_string, tokens, flags,
				(*input)[*pos]);
	else
	{
		if (return_string)
			exit_status = add_token(tokens, TOKEN_STRING, return_string);
	}
	free_2_str_and_nullify(&flags->var, &flags->string);
	return (success_or_mem_error(exit_status));
}

int	tokenize_parenthesis(const char *input, size_t *pos, t_token_list **tokens,
		t_token_flags *flags)
{
	int	status;

	if (input[*pos] == '(')
	{
		flags->paren_count++;
		status = add_token_1_pos(pos, tokens, TOKEN_LEFT_PARENTHESIS, flags);
	}
	else
	{
		flags->paren_count--;
		status = add_token_1_pos(pos, tokens, TOKEN_RIGHT_PARENTHESIS, flags);
	}
	return (success_or_mem_error(status));
}
