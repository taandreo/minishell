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
			exit_status = add_special_or_string(flags->string, tokens,
					*input, *pos);
	}
	free_2_str_and_nullify(&flags->var, &flags->string);
	return (exit_status);
}

int	tokenize_redirections(char *input, size_t *pos,
		t_token_list **tokens, t_token_flags *flags)
{
	flags->is_command = false;
	flags->is_redirection = true;
	if (!tokens || !*tokens)
		return (MISUSE);
	if (input[*pos] == '<' && peek_next(input, *pos, flags->input_len) == '<')
		return (add_token_2_pos(pos, tokens, TOKEN_REDIRECTION_HEREDOC, flags));
	if (input[*pos] == '<')
		return (add_token_1_pos(pos, tokens, TOKEN_REDIRECTION_INPUT, flags));
	if (input[*pos] == '>' && peek_next(input, *pos, flags->input_len) == '>')
		return (add_token_2_pos(pos, tokens, TOKEN_REDIRECTION_APPEND, flags));
	return (add_token_1_pos(pos, tokens, TOKEN_REDIRECTION_OUTPUT, flags));
}

int	tokenize_operators(char *input, size_t *pos,
		t_token_list **tokens, t_token_flags *flags)
{
	flags->is_command = true;
	flags->is_redirection = false;
	flags->has_command = false;
	if (!tokens || !*tokens)
		return (MISUSE);
	if (input[*pos] == '|' && peek_next(input, *pos, flags->input_len) == '|')
		return (add_token_2_pos(pos, tokens, TOKEN_OR, flags));
	if (input[*pos] == '|')
		return (add_token_1_pos(pos, tokens, TOKEN_PIPE, flags));
	if (input[*pos] == '&' && peek_next(input, *pos, flags->input_len) == '&')
		return (add_token_2_pos(pos, tokens, TOKEN_AND, flags));
	return (add_token_1_pos(pos, tokens, TOKEN_INVALID, flags));
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
			exit_status = add_special_or_string(return_string, tokens, *input,
					*pos);
	}
	free_2_str_and_nullify(&flags->var, &flags->string);
	return (exit_status);
}

int	tokenize_parenthesis(char *input, size_t *pos, t_token_list **tokens,
		t_token_flags *flags)
{
	if (input[*pos] == '(')
	{
		flags->paren_count++;
		return (add_token_1_pos(pos, tokens, TOKEN_LEFT_PARENTHESIS, flags));
	}
	flags->paren_count--;
	return (add_token_1_pos(pos, tokens, TOKEN_RIGHT_PARENTHESIS, flags));
}
