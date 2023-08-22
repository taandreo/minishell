#include "minishell_bonus.h"

int	tokenize_quotes(const char *input, size_t *position,
		t_token_list *tokens, t_token_flags *flags);
int	tokenize_redirections(const char *input, size_t *position,
		t_token_list *tokens, t_token_flags *flags);
int	tokenize_operators(const char *input, size_t *position,
		t_token_list *tokens, t_token_flags *flags);
int	tokenize_strings(const char *input, size_t *position,
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
			tokenize_quotes(input, &position, tokens, flags);
		else if (c == '<' || c == '>')
			tokenize_redirections(input, &position, tokens, flags);
		else if (c == '&' || c == '|')
			tokenize_operators(input, &position, tokens, flags);
		else
			tokenize_strings(input, &position, tokens, flags);
	}
	add_token(tokens, TOKEN_END, "");
	return (tokens);
}

int	tokenize_quotes(const char *input, size_t *position,
		t_token_list *tokens, t_token_flags *flags)
{
	char	*quoted_string;
	int 	exit_status;

	if (!tokens)
		return (MISUSE);
	quoted_string = handle_quotes(input, position, tokens);
	if (!quoted_string)
	{
		free_token_list(tokens);
		tokens = NULL;
		return (MISUSE);
	}
	if (flags->is_command)
		exit_status = add_builtin_or_command(quoted_string, tokens, flags);
	else
		exit_status = add_token(tokens, TOKEN_STRING, quoted_string);
	free(quoted_string);
	return (exit_status);
}

int	tokenize_redirections(const char *input, size_t *pos,
		t_token_list *tokens, t_token_flags *flags)
{
	flags->is_command = false;
	flags->is_redirection = true;
	if (!tokens)
		return (MISUSE);
	if (input[*pos] == '<' && peek_next(input, *pos, flags->input_len) == '<')
		return (add_token_2_pos(pos, tokens, TOKEN_REDIRECTION_HEREDOC, flags));
	else if (input[*pos] == '<')
		return (add_token_1_pos(pos, tokens, TOKEN_REDIRECTION_INPUT, flags));
	else if (input[*pos] == '>'
		&& peek_next(input, *pos, flags->input_len) == '>')
		return (add_token_2_pos(pos, tokens, TOKEN_REDIRECTION_APPEND, flags));
	return (add_token_1_pos(pos, tokens, TOKEN_REDIRECTION_OUTPUT, flags));
}

int	tokenize_operators(const char *input, size_t *pos,
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

int	tokenize_strings(const char *input, size_t *pos,
		t_token_list *tokens, t_token_flags *flags)
{
	char		*return_string;
	int			status;

	return_string = get_string_from_input(input, pos, tokens);
	if (!return_string)
	{
		if (tokens)
			free_token_list(tokens);
		return (2);
	}
	if (flags->is_command)
		status = add_builtin_or_command(return_string, tokens, flags);
	else if (flags->is_redirection)
		status = add_filename_or_string(return_string, tokens, flags);
	else
	{
		if (tokens->tail->token.type == TOKEN_ECHO
			&& ft_strcmp(return_string, "-n") == 0)
			status = add_token(tokens, TOKEN_SPECIAL_ARG, return_string);
		else
			status = add_token(tokens, TOKEN_STRING, return_string);
	}
	free(return_string);
	return (status);
}



