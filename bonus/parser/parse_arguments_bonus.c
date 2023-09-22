#include "minishell_bonus.h"

t_string	*get_next_string(t_token_list *tokens);

t_arguments	*parse_arguments(t_token_list *tokens)
{
	t_arguments	*args;

	args = NULL;
	if (current_token_type(tokens) == TOKEN_END)
		return (NULL);
	if (current_token_type(tokens) == TOKEN_SPACE)
		advance_token(tokens);
	if (current_token_type(tokens) == TOKEN_STRING
			|| current_token_type(tokens) == TOKEN_EXIT_CODE
			|| current_token_type(tokens) == TOKEN_WILDCARD)
	{
		args = malloc(sizeof(t_arguments));
		if (!args)
			return (return_mem_alloc_error());
		args->type = current_token_type(tokens);
		args->string = parse_string(tokens);
		args->next = parse_arguments(tokens);
	}
	return (args);
}

t_string	*parse_string(t_token_list *tokens)
{
	t_string	*string;

	string = malloc(sizeof(t_string));
	if (!string)
	{
		free_string(string);
		return (return_mem_alloc_error());
	}
	string->type = current_token_type(tokens);
	string->value = ft_strdup(current_token(tokens).value);
	if (!string->value)
	{
		free_string(string);
		return (return_mem_alloc_error());
	}
	advance_token(tokens);
	string->next = get_next_string(tokens);
	return (string);
}

t_string	*get_next_string(t_token_list *tokens)
{
	t_string	*string;

	string = NULL;
	if (current_token_type(tokens) != TOKEN_SPACE
			&& !(current_token_type(tokens) >= TOKEN_AND
			&& current_token_type(tokens) <= TOKEN_ERROR)
			&& current_token_type(tokens) != TOKEN_END)
		string = parse_string(tokens);
	return (string);
}
