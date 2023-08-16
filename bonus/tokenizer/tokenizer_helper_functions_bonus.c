#include "minishell_bonus.h"

char	peek_next(const char *input, size_t position, size_t input_len)
{
	if (position + 1 < input_len)
		return (input[position + 1]);
	else
		return '\0';
}

t_bool is_string_start(char c)
{
	return (!ft_is_space(c) && !ft_strchr("><|&", c));
}

t_bool	has_quotes(char c)
{
	return (c == '\'' || c == '\"');
}

t_token_flags init_flags(size_t input_len)
{
	t_token_flags flags;

	flags.input_len = input_len;
	flags.is_redirection = false;
	flags.is_command = true;
	flags.has_command = false;
	return (flags);
}
