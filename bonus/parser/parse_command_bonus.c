/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:32:46 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:32:47 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

t_grouping		*parse_grouping(t_command_part *command_part,
					t_token_list *tokens, t_parser_state *state);
t_pipeline		*parse_pipeline(t_token_list *tokens, t_parser_state *state);
t_conjunctions	*parse_conjuctions(t_token_list *tokens, t_parser_state *state);
t_command_part	*parse_command_part(t_token_list *tokens, t_parser_state *state);

t_command	*parse_command(t_token_list *tokens, t_parser_state *state)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (state->error)
	{
		free_command(command);
		return (NULL);
	}
	if (!command)
		return (return_mem_alloc_error());
	command->pipeline = parse_pipeline(tokens, state);
	if (!command->pipeline)
		return (free_and_return_null(command));
	command->conjunctions = parse_conjuctions(tokens, state);
	return (command);
}

t_pipeline	*parse_pipeline(t_token_list *tokens, t_parser_state *state)
{
	t_pipeline	*pipeline;

	pipeline = malloc(sizeof(t_pipeline));
	if (!pipeline)
		return (return_mem_alloc_error());
	pipeline->type = TOKEN_NONE;
	pipeline->cmd_part = parse_command_part(tokens, state);
	if (!pipeline->cmd_part)
		return (free_and_return_null(pipeline));
	pipeline->next = NULL;
	if (current_token_type(tokens) == TOKEN_PIPE)
	{
		pipeline->type = TOKEN_PIPE;
		advance_token(tokens);
		pipeline->next = parse_pipeline(tokens, state);
		if (!pipeline->next)
		{
			free_command_part(pipeline->cmd_part);
			return (free_and_return_null(pipeline));
		}
	}
	return (pipeline);
}

t_conjunctions	*parse_conjuctions(t_token_list *tokens, t_parser_state *state)
{
	t_conjunctions	*conjunctions;

	if (current_token_type(tokens) != TOKEN_AND
		&& current_token_type(tokens) != TOKEN_OR)
		return (NULL);
	conjunctions = malloc(sizeof(t_conjunctions));
	if (!conjunctions)
		return (return_mem_alloc_error());
	conjunctions->type = current_token_type(tokens);
	advance_token(tokens);
	conjunctions->pipeline = parse_pipeline(tokens, state);
	if (!conjunctions->pipeline)
		return (free_and_return_null(conjunctions));
	conjunctions->next = parse_conjuctions(tokens, state);
	return (conjunctions);
}

t_command_part	*parse_command_part(t_token_list *tokens, t_parser_state *state)
{
	t_command_part	*command_part;
	t_redirections	*initial_redirections;

	command_part = malloc(sizeof(t_command_part));
	if (!command_part)
		return (return_mem_alloc_error());
	init_command_part_fields(command_part);
	initial_redirections = parse_redirections(tokens, state);
	if (current_token_type(tokens) == TOKEN_LEFT_PARENTHESIS)
		command_part->u_cmd.grouping = parse_grouping(command_part, tokens,
				state);
	else if (current_token_type(tokens) >= TOKEN_ECHO && current_token_type(tokens) <= TOKEN_EXIT)
		command_part->u_cmd.builtin_cmd = handle_builtin_tokens(command_part, tokens);
	else if (current_token_type(tokens) == TOKEN_COMMAND_NAME)
		command_part->u_cmd.cmd_name = handle_command_name_tokens(command_part, tokens);
	else if (initial_redirections)
	{
		command_part->type = TOKEN_REDIRECTIONS;
		command_part->redirections = initial_redirections;
	}
	else
	{
		free_command_part(command_part);
		return (NULL);
	}
	if (state->error)
	{
		free_command_part(command_part);
		return (NULL);
	}
	subsequent_redirections(command_part, initial_redirections, tokens, state);
	return (command_part);
}

t_grouping	*parse_grouping(t_command_part *command_part, t_token_list *tokens,
		t_parser_state *state)
{
	t_grouping	*grouping;

	command_part->type = TOKEN_GROUP;
	state->paren_count++;
	grouping = malloc(sizeof(t_grouping));
	if (!grouping)
		return (return_mem_alloc_error());
	advance_token(tokens);
	grouping->enclosed_cmd = parse_command(tokens, state);
	if (!grouping->enclosed_cmd)
		return (free_and_return_null(grouping));
	if (current_token_type(tokens) != TOKEN_RIGHT_PARENTHESIS)
	{
		free_grouping(grouping);
		return (NULL);
	}
	state->paren_count--;
	advance_token(tokens);
	if (!is_operator_or_end(current_token_type(tokens)))
	{
		if (state->paren_count > 0
			&& current_token_type(tokens) == TOKEN_RIGHT_PARENTHESIS)
			return (grouping);
		free_grouping(grouping);
		return (NULL);
	}
	return (grouping);
}
