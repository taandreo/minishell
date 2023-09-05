#include "minishell_bonus.h"

int	tokenize_by_category(const char *input, size_t *position,
		t_token_list **tokens, t_token_flags *flags)
{
	char	c;

	c = input[*position];
	if (has_quotes(c))
		return (tokenize_quotes(input, position, tokens, flags));
	else if (c == '<' || c == '>')
		return (tokenize_redirections(input, position, tokens, flags));
	else if (c == '&' || c == '|')
		return (tokenize_operators(input, position, tokens, flags));
	return (tokenize_strings(input, position, tokens, flags));
}

int	tokenize_quotes(const char *input, size_t *position,
		t_token_list **tokens, t_token_flags *flags)
{
	char			*quoted_string;
	int				exit_status;

	if (!tokens || !*tokens)
		return (MISUSE);
	quoted_string = handle_quotes(input, position, tokens, flags);
	if (!quoted_string)
		return (misuse_or_unclosed_quotes_error(tokens));
	if (flags->is_command)
		exit_status = add_builtin_or_command(quoted_string, tokens, flags);
	else if (flags->is_redirection)
		exit_status = add_filename_or_string(quoted_string, tokens, flags);
	else
		exit_status = add_token(tokens, TOKEN_STRING, quoted_string);
	if (flags->var)
		free_str_and_nullify(&flags->var);
	free(quoted_string);
	return (exit_status);
}

int	tokenize_redirections(const char *input, size_t *pos,
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

int	tokenize_operators(const char *input, size_t *pos,
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

int	tokenize_strings(const char *input, size_t *pos,
		t_token_list **tokens, t_token_flags *flags)
{
	char		*return_string;
	int			exit_status;

	if (!tokens || !*tokens)
		return (MISUSE);
	return_string = get_string_from_input(input, pos, tokens, flags);
	if (!return_string)
		return (misuse_or_unclosed_quotes_error(tokens));
	if (flags->is_command)
		exit_status = add_builtin_or_command(return_string, tokens, flags);
	else if (flags->is_redirection)
		exit_status = add_filename_or_string(return_string, tokens, flags);
	else
	{
		if ((*tokens)->tail->token.type == TOKEN_ECHO
			&& ft_strcmp(return_string, "-n") == 0)
			exit_status = add_token(tokens, TOKEN_SPECIAL_ARG, return_string);
		else
			exit_status = add_token(tokens, TOKEN_STRING, return_string);
	}
	if (flags->var)
		free_str_and_nullify(&flags->var);
	free(return_string);
	return (exit_status);
}
