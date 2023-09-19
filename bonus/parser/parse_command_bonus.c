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

t_grouping		*parse_grouping(t_token_list *tokens);
t_pipeline		*parse_pipeline(t_token_list *tokens);
t_conjunctions	*parse_conjuctions(t_token_list *tokens);
t_command_part	*parse_command_part(t_token_list *tokens);

t_command	*parse_command(t_token_list *tokens)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (!command)
		return (return_mem_alloc_error());
	if (current_token_type(tokens) == TOKEN_LEFT_PARENTHESIS)
	{
		command->grouping = parse_grouping(tokens);
		command->pipeline = NULL;
		if (!command->grouping)
			free_and_return_null(command);
	}
	else
	{
		command->pipeline = parse_pipeline(tokens);
		command->grouping = NULL;
		if (!command->pipeline)
			return (free_and_return_null(command));
	}
	command->conjunctions = parse_conjuctions(tokens);
	return (command);
}

t_grouping *parse_grouping(t_token_list *tokens)
{
	t_grouping	*grouping;

	if (current_token_type(tokens) != TOKEN_LEFT_PARENTHESIS)
		return (NULL);
	grouping = malloc(sizeof(t_grouping));
	if (!grouping)
		return (return_mem_alloc_error());
	advance_token(tokens);
	grouping->enclosed_cmd = parse_command(tokens);
	if (!grouping->enclosed_cmd)
		return (free_and_return_null(grouping));
	if (current_token_type(tokens) != TOKEN_RIGHT_PARENTHESIS)
	{
		free_grouping(grouping);
		return (return_syntax_error(current_token(tokens).value));
	}
	advance_token(tokens);
	grouping->conjunctions = NULL;
	grouping->next_grouping = NULL;
	if (current_token_type(tokens) == TOKEN_AND || current_token_type(tokens) == TOKEN_OR)
	{
		grouping->conjunctions = parse_conjuctions(tokens);
		if (grouping->conjunctions)
			grouping->next_grouping = parse_grouping(tokens);
	}
	return (grouping);
}

t_pipeline	*parse_pipeline(t_token_list *tokens)
{
	t_pipeline	*pipeline;

	pipeline = malloc(sizeof(t_pipeline));
	if (!pipeline)
		return (return_mem_alloc_error());
	pipeline->cmd_part = parse_command_part(tokens);
	if (!pipeline->cmd_part)
		return (free_and_return_null(pipeline));
	pipeline->next = NULL;
	if (current_token_type(tokens) == TOKEN_PIPE)
	{
		advance_token(tokens);
		pipeline->next = parse_pipeline(tokens);
		if (!pipeline->next)
		{
			free_command_part(pipeline->cmd_part);
			return (free_and_return_null(pipeline));
		}
	}
	return (pipeline);
}

t_conjunctions	*parse_conjuctions(t_token_list *tokens)
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
	conjunctions->pipeline = parse_pipeline(tokens);
	if (!conjunctions->pipeline)
		return (free_and_return_null(conjunctions));
	conjunctions->next = parse_conjuctions(tokens);
	return (conjunctions);
}

t_command_part	*parse_command_part(t_token_list *tokens)
{
	t_command_part	*command_part;
	t_redirections	*initial_redirections;

	command_part = malloc(sizeof(t_command_part));
	if (!command_part)
		return (return_mem_alloc_error());
	init_command_part_fields(command_part);
	initial_redirections = parse_redirections(tokens);
	if (current_token_type(tokens) >= TOKEN_ECHO
		&& current_token_type(tokens) <= TOKEN_EXIT)
		command_part = handle_builtin_tokens(command_part, tokens);
	else if (current_token_type(tokens) == TOKEN_COMMAND_NAME)
		command_part = handle_command_name_tokens(command_part, tokens);
	else if (initial_redirections)
	{
		command_part->type = TOKEN_REDIRECTIONS;
		command_part->redirections = initial_redirections;
	}
	else
	{
		free_command_part(command_part);
		return (return_syntax_error(current_token(tokens).value));
	}
	add_subsequent_redirections_to_initial(command_part, initial_redirections, tokens);
	return (command_part);
}
