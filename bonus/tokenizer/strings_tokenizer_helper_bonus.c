#include "minishell_bonus.h"

char	*get_string_from_input(const char *input, size_t *pos,
				t_token_list *tokens)
{
	char		*return_string;
	const char	*start;

	start = input + *pos;
	while (input[*pos] && is_string_start(input[*pos]))
	{
		if (input[*pos] == '\'' || input[*pos] == '\"')
		{

			(*pos)--;
		}
		(*pos)++;
	}
	return_string = ft_strndup(start, input + *pos - start);
	if (!return_string)
		return (return_mem_alloc_error());
	return (return_string);
}

handle_quotes(input, pos, tokens);

int	add_builtin_or_command(char *string, t_token_list *tokens,
		t_token_flags *flags)
{
	int exit_status;

	if (is_builtin(string))
		exit_status = add_token(tokens, get_builtin_token(string),string);
	else
		exit_status = add_token(tokens, TOKEN_COMMAND_NAME, string);
	flags->is_command = false;
	flags->has_command = true;
	return (exit_status);
}

int	add_filename_or_string(char *string, t_token_list *tokens,
		t_token_flags *flags)
{
	int	exit_status;

	if (flags->has_heredoc)
		exit_status = add_token(tokens, TOKEN_STRING, string);
	else
		exit_status = add_token(tokens, TOKEN_FILENAME, string);
	flags->is_redirection = false;
	if (!flags->has_command)
		flags->is_command = true;
	return (exit_status);
}
