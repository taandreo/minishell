/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_arguments_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 19:40:50 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/15 19:40:51 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_arguments	*parse_arguments(t_token_list *tokens, t_parser_state *state)
{
	t_arguments	*args;

	args = NULL;
	if (current_token_type(tokens) == TOKEN_SPACE)
		advance_token(tokens);
	if (current_token_type(tokens) == TOKEN_STRING
		|| current_token_type(tokens) == TOKEN_EXIT_CODE
		|| current_token_type(tokens) == TOKEN_WILDCARD)
	{
		args = malloc(sizeof(t_arguments));
		if (!args)
			return (print_misuse_state_error(state));
		args->type = current_token_type(tokens);
		args->string = parse_string(tokens, state);
		if (state->status == MISUSE && state->error)
		{
			free_arguments(args);
			return (print_misuse_state_error(state));
		}
		args->next = parse_arguments(tokens, state);
	}
	return (args);
}

t_string	*parse_string(t_token_list *tokens, t_parser_state *state)
{
	t_string	*string;

	string = NULL;
	if (current_token_type(tokens) != TOKEN_SPACE
		&& !(is_operator_or_invalid_token(current_token_type(tokens)))
		&& current_token_type(tokens) != TOKEN_END)
	{
		string = malloc(sizeof(t_string));
		if (!string)
			return (misuse_state_error(state));
		string->type = current_token_type(tokens);
		string->value = ft_strdup(current_token(tokens).value);
		if (!string->value)
		{
			free_string(string);
			return (misuse_state_error(state));
		}
		advance_token(tokens);
		string->next = parse_string(tokens, state);
	}
	return (string);
}

void	subsequent_arguments(t_command_part *command_part, t_token_list *tokens,
			t_parser_state *state)
{
	t_arguments	*arguments;
	t_arguments	*last_argument;

	arguments = parse_arguments(tokens, state);
	if (!arguments)
		return ;
	if (command_part->arguments)
	{
		last_argument = command_part->arguments;
		while (last_argument->next)
			last_argument = last_argument->next;
		last_argument->next = arguments;
	}
	else
		command_part->arguments = arguments;
}
