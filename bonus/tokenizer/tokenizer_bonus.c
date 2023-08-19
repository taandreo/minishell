#include "minishell_bonus.h"

void	tokenize_quotes(const char *input, size_t *position,
			t_token_list *tokens, t_token_flags *flags);
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
			tokenize_quotes(input, &position, &tokens, flags);
		else if (c == '<' || c == '>')
			tokenize_redirections(input, &position, &tokens, flags);
		else if (c == '&' || c == '|')
			tokenize_operators(input, &position, &tokens, flags);
		else
			tokenize_strings(input, &position, &tokens, flags);
	}
	return (tokens);
}

void	tokenize_quotes(const char *input, size_t *position,
			t_token_list *tokens, t_token_flags *flags)
{
	char	*quoted_string;

	quoted_string = handle_quotes(input, position, tokens);
	if (flags->is_command)
	{
		if (is_builtin(quoted_string))
			add_token(tokens, get_builtin_token(quoted_string), quoted_string);
		else
			add_token(tokens, TOKEN_COMMAND_NAME, quoted_string);
	}
	else
		add_token(tokens, TOKEN_STRING, quoted_string);
	free(quoted_string);
}

void	tokenize_redirections(const char *input, size_t *pos,
			t_token_list *tokens, t_token_flags *flags)
{
	flags->is_command = false;
	flags->is_redirection = true;
	flags->has_heredoc = !flags->has_heredoc;
	if (input[*pos] == '<' && peek_next(input, *pos, flags->input_len) == '<')
	{
		*pos += 2;
		add_token(tokens, TOKEN_REDIRECTION_HEREDOC, "<<");
		flags->has_heredoc = true;
	}
	else if (input[*pos] == '<')
	{
		(*pos)++;
		add_token(tokens, TOKEN_REDIRECTION_INPUT, "<");
	}
	else if (input[*pos] == '>'
		&& peek_next(input, *pos, flags->input_len) == '>')
	{
		*pos += 2;
		add_token(tokens, TOKEN_REDIRECTION_APPEND, ">>");
	}
	else if (input[*pos] == '>')
	{
		(*pos)++;
		add_token(tokens, TOKEN_REDIRECTION_OUTPUT, ">");
	}
}

void	tokenize_operators(const char *input, size_t *pos,
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

void	tokenize_strings(const char *input, size_t *pos,
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
		add_token(tokens, TOKEN_STRING, return_string);
	free(return_string);
}
