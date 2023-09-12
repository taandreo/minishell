#include "minishell_bonus.h"

char	peek_next(const char *input, size_t position, size_t input_len)
{
	if (position + 1 < input_len)
		return (input[position + 1]);
	else
		return ('\0');
}

t_bool	is_string_start(char c, t_token_flags *flags)
{
	if (!flags->init_var)
	{
		if (flags->var_len > -1)
			return (!ft_is_space(c));
	}
	return (!ft_is_space(c) && !ft_strchr("><|&", c));
}

t_bool	has_quotes(char c)
{
	return (c == '\'' || c == '\"');
}

t_token_flags	init_flags(size_t input_len)
{
	t_token_flags	flags;

	flags.input_len = input_len;
	flags.var_len = -1;
	flags.is_redirection = false;
	flags.is_command = true;
	flags.has_command = false;
	flags.has_heredoc = false;
	flags.inside_quotes = false;
	flags.init_var = false;
	flags.quote_type = '\0';
	flags.string = NULL;
	flags.var = NULL;
	return (flags);
}

t_token_type	get_builtin_token(char *token)
{
	if (ft_strcmp(token, "echo") == 0)
		return (TOKEN_ECHO);
	else if (ft_strcmp(token, "cd") == 0)
		return (TOKEN_CD);
	else if (ft_strcmp(token, "pwd") == 0)
		return (TOKEN_PWD);
	else if (ft_strcmp(token, "export") == 0)
		return (TOKEN_EXPORT);
	else if (ft_strcmp(token, "unset") == 0)
		return (TOKEN_UNSET);
	else if (ft_strcmp(token, "env") == 0)
		return (TOKEN_ENV);
	else
		return (TOKEN_EXIT);
}
