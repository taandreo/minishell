#include "minishell_bonus.h"

t_pipeline		*parse_pipeline(t_token_list *tokens);
t_conjunctions	*parse_conjuctions(t_token_list *tokens);
t_command_part	*parse_command_part(t_token_list *tokens);

t_command	*parse_command(t_token_list *tokens)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (!command)
		return (return_mem_alloc_error());
	command->pipeline = parse_pipeline(tokens);
	if (!command->pipeline)
		return (free_and_return_null(command));
	command->conjunctions = parse_conjuctions(tokens);
	return (command);
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
//	t_redirections	*initial_redirections;

	command_part = malloc(sizeof(t_command_part));
	if (!command_part)
		return (return_mem_alloc_error());
	init_command_part_fields(command_part);
//	initial_redirections = parse_redirections(tokens);
	if (current_token_type(tokens) >= TOKEN_ECHO
		&& current_token_type(tokens) <= TOKEN_EXIT)
		command_part = handle_builtin_tokens(command_part, tokens);
	else if (current_token_type(tokens) == TOKEN_COMMAND_NAME)
		command_part = handle_command_name_tokens(command_part, tokens);
//	else if (initial_redirections)
//	{
//		command_part->type = TOKEN_REDIRECTIONS;
//		command_part->redirections = initial_redirections;
//	}
//	else
//	{
//		free_command_part(command_part);
//		return (return_syntax_error(current_token(tokens).value));
//	}
//	add_subsequent_redirections_to_initial(command_part, initial_redirections);
	return (command_part);
}

