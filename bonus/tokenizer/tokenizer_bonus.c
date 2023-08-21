#include "minishell_bonus.h"

t_token_list	*tokenize_quotes(const char *input, size_t *position,
					t_token_list *tokens, t_token_flags *flags);
t_token_list	*tokenize_redirections(const char *input, size_t *position,
					t_token_list *tokens, t_token_flags *flags);
t_token_list	*tokenize_operators(const char *input, size_t *position,
					t_token_list *tokens, t_token_flags *flags);
t_token_list	*tokenize_strings(const char *input, size_t *position,
					t_token_list *tokens, t_token_flags *flags);

t_token_list	*tokenizer(char *input, t_token_flags *flags)
{
	t_token_list	*tokens;
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
			tokens = tokenize_quotes(input, &position, tokens, flags);
		else if (c == '<' || c == '>')
			tokens = tokenize_redirections(input, &position, tokens, flags);
		else if (c == '&' || c == '|')
			tokens = tokenize_operators(input, &position, tokens, flags);
		else
			tokens = tokenize_strings(input, &position, tokens, flags);
	}
	tokens = add_token(tokens, TOKEN_END, "");
	return (tokens);
}

t_token_list	*tokenize_quotes(const char *input, size_t *position,
					t_token_list *tokens, t_token_flags *flags)
{
	char	*quoted_string;

	quoted_string = handle_quotes(input, position, tokens);
	if (!quoted_string)
	{
		if (tokens)
			free_token_list(tokens);
		return (NULL);
	}
	if (flags->is_command)
	{
		if (is_builtin(quoted_string))
			tokens = add_token(tokens,
						get_builtin_token(quoted_string), quoted_string);
		else
			tokens = add_token(tokens, TOKEN_COMMAND_NAME, quoted_string);
		flags->is_command = false;
	}
	else
		tokens = add_token(tokens, TOKEN_STRING, quoted_string);
	free(quoted_string);
	return (*tokens);
}

t_token_list	*tokenize_redirections(const char *input, size_t *pos,
					t_token_list *tokens, t_token_flags *flags)
{
	flags->is_command = false;
	flags->is_redirection = true;
	if (input[*pos] == '<' && peek_next(input, *pos, flags->input_len) == '<')
	{
		move_2_pos_and_add_token(pos, tokens, TOKEN_REDIRECTION_HEREDOC, "<<");
		flags->has_heredoc = true;
	}
	else if (input[*pos] == '<')
	{
		move_1_pos_and_add_token(pos, tokens, TOKEN_REDIRECTION_INPUT, "<");
		flags->has_heredoc = false;
	}
	else if (input[*pos] == '>'
		&& peek_next(input, *pos, flags->input_len) == '>')
	{
		move_2_pos_and_add_token(pos, tokens, TOKEN_REDIRECTION_APPEND, ">>");
		flags->has_heredoc = false;
	}
	else if (input[*pos] == '>')
	{
		move_1_pos_and_add_token(pos, tokens, TOKEN_REDIRECTION_OUTPUT, ">");
		flags->has_heredoc = false;
	}
}

t_token_list	*tokenize_operators(const char *input, size_t *pos,
					t_token_list *tokens, t_token_flags *flags)
{
	flags->is_command = true;
	flags->is_redirection = false;
	flags->has_command = false;
	if (input[*pos] == '|' && peek_next(input, *pos, flags->input_len) == '|')
	{
		*pos += 2;
		add_token(tokens, TOKEN_OR, "||");
	}
	else if (input[*pos] == '|')
	{
		(*pos)++;
		add_token(tokens, TOKEN_PIPE, "|");
	}
	else if (input[*pos] == '&'
		&& peek_next(input, *pos, flags->input_len) == '&')
	{
		*pos += 2;
		add_token(tokens, TOKEN_AND, "&&");
	}
	else
	{
		(*pos)++;
		add_token(tokens, TOKEN_INVALID, "&");
	}
}

t_token_list	*tokenize_strings(const char *input, size_t *pos,
					t_token_list *tokens, t_token_flags *flags)
{
	char		*return_string;

	return_string = get_string_from_input(input, pos, tokens);
	if (!return_string)
		return ;
	if (flags->is_command)
		add_builtin_or_command(return_string, tokens, flags);
	else if (flags->is_redirection)
		add_filename_or_string(return_string, tokens, flags);
	else
	{
		if (tokens->tail->token.type == TOKEN_ECHO
			&& ft_strcmp(return_string, "-n") == 0)
			add_token(tokens, TOKEN_SPECIAL_ARG, return_string);
		else
			add_token(tokens, TOKEN_STRING, return_string);
	}
	free(return_string);
}



