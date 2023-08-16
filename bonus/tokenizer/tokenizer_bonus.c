#include "minishell_bonus.h"

char	*handle_quotes(const char *input, size_t *position,
			t_token_list *tokens);
void	tokenize_redirections(const char *input, size_t *position,
			t_token_list *tokens, t_token_flags *flags);
void	tokenize_operators(const char *input, size_t *position,
			t_token_list *tokens, t_token_flags *flags);
void	tokenize_strings(const char *input, size_t *position,
			t_token_list *tokens, t_token_flags *flags);

t_token_list	tokenizer(char *input, t_token_flags *flags)
{
	t_token_list	tokens;
	size_t			position;
	char			c;

	position = 0;
	tokens = create_token_list();
	while (position < flags->input_len)
	{
		c = input[position];
		if (ft_is_space(c))
		{
			position++;
			continue ;
		}
		if (has_quotes(c))
			add_token(&tokens, TOKEN_STRING,
				handle_quotes(input, &position, &tokens));
		else if (c == '<' || c == '>')
			tokenize_redirections(input, &position, &tokens, flags);
		else if (c == '&' || c == '|')
			tokenize_operators(input, &position, &tokens, flags);
		else
			tokenize_strings(input, &position, &tokens, flags);
	}
	return (tokens);
}

char	*handle_quotes(const char *input, size_t *position,
			t_token_list *tokens)
{
	char	quote_type;
	size_t	start;
	size_t	length;
	char	*quoted_string;

	quote_type = input[*position];
	start = *position + 1;
	(*position)++;
	while (input[*position] && input[*position] != quote_type)
	{
		if (quote_type == '\"' && input[*position] == '$')
		{
			// TODO: Variable Expansion
			(*position)++;
		}
		(*position)++;
	}
	if (input[*position] != quote_type)
	{
		add_token(tokens, TOKEN_ERROR, "Error: Unclosed quotes.");
		return (NULL);
	}
	length = *position - start;
	quoted_string = ft_strndup(input + start, length);
	if (!quoted_string)
	{
		add_token(tokens, TOKEN_ERROR, "Error: Unable to alloc memory");
		return (NULL);
	}
	(*position)++;
	return (quoted_string);
}

void	tokenize_redirections(const char *input, size_t *position,
			t_token_list *tokens, t_token_flags *flags)
{
	flags->is_command = false;
	flags->is_redirection = true;
	if (input[*position] == '<'
		&& peek_next(input, *position, flags->input_len) == '<')
	{
		*position += 2;
		add_token(tokens, TOKEN_REDIRECTION_HEREDOC, "<<");
	}
	else if (input[*position] == '<')
	{
		(*position)++;
		add_token(tokens, TOKEN_REDIRECTION_INPUT, "<");
	}
	else if (input[*position] == '>'
		&& peek_next(input, *position, flags->input_len) == '>')
	{
		*position += 2;
		add_token(tokens, TOKEN_REDIRECTION_APPEND, ">>");
	}
	else if (input[*position] == '>')
	{
		(*position)++;
		add_token(tokens, TOKEN_REDIRECTION_OUTPUT, ">");
	}
}

void	tokenize_operators(const char *input, size_t *position,
				t_token_list *tokens, t_token_flags *flags)
{
	flags->is_command = true;
	flags->is_redirection = false;
	if (input[*position] == '|'
		&& peek_next(input, *position, flags->input_len) == '|')
	{
		*position += 2;
		add_token(tokens, TOKEN_OR, "||");
	}
	else if (input[*position] == '|')
	{
		(*position)++;
		add_token(tokens, TOKEN_PIPE, "|");
	}
	else if (input[*position] == '&'
		&& peek_next(input, *position, flags->input_len) == '&')
	{
		*position += 2;
		add_token(tokens, TOKEN_AND, "&&");
	}
	else
	{
		(*position)++;
		add_token(tokens, TOKEN_INVALID, "&");
	}
}

void	tokenize_strings(const char *input, size_t *position,
				t_token_list *tokens, t_token_flags *flags)
{
	const char	*start;
	char		*return_string;

	start = input + *position;
	while (input[*position] && is_string_start(input[*position]))
	{
		if (input[*position] == '\'' || input[*position] == '\"')
			handle_quotes(input, position, tokens);
		(*position)++;
	}
	return_string = ft_strndup(start, input + *position - start);
	if (!return_string)
		add_token(tokens, TOKEN_ERROR, "Error: Failed to allocate memory.");
	if (flags->is_command)
	{
		add_token(tokens, TOKEN_COMMAND_NAME, return_string);
		flags->is_command = false;
	}
	else if (flags->is_redirection)
	{
		add_token(tokens, TOKEN_FILENAME, return_string);
		flags->is_redirection = false;
	}
	else
		add_token(tokens, TOKEN_STRING, return_string);
	free(return_string);
}
