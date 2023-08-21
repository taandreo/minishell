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
			handle_quotes(input, pos, tokens);
			(*pos)--;
		}
		(*pos)++;
	}
	return_string = ft_strndup(start, input + *pos - start);
	if (!return_string)
	{
		free_token_list(tokens);
		return (return_mem_alloc_error());
	}
	return (return_string);
}

void	add_builtin_or_command(char *return_string, t_token_list *tokens,
				t_token_flags *flags)
{
	if (is_builtin(return_string))
		add_token(tokens, get_builtin_token(return_string), return_string);
	else
		add_token(tokens, TOKEN_COMMAND_NAME, return_string);
	flags->is_command = false;
	flags->has_command = true;
}

void	add_filename_or_string(char *return_string, t_token_list *tokens,
				t_token_flags *flags)
{
	if (flags->has_heredoc)
		add_token(tokens, TOKEN_STRING, return_string);
	else
		add_token(tokens, TOKEN_FILENAME, return_string);
	flags->is_redirection = false;
	if (!flags->has_command)
		flags->is_command = true;
}
