#include "minishell_bonus.h"

char			*extract_quoted_string(const char *input, size_t *pos,
					t_token_list **tokens, t_token_flags *flags);

char	*handle_quotes(const char *input, size_t *position,
			t_token_list **tokens, t_token_flags *flags)
{
	char	*quoted_string;
	char	*subsequent_string;

	flags->quote_type = input[*position];
	if (!flags->string)
		flags->string = ft_strdup("");
	flags->handle_quotes = true;
	quoted_string = extract_quoted_string(input, position, tokens, flags);
	subsequent_string = NULL;
	if (input[*position] != flags->quote_type)
		return (add_unclosed_quotes_token(tokens, flags, quoted_string));
	if (!quoted_string)
		return (return_mem_alloc_error());
	(*position)++;
	if (input[*position] && is_string_start(input[*position]))
	{
		subsequent_string = get_string_from_input(input, position, tokens,
				flags);
		if (!subsequent_string)
			return (free_and_return_null(quoted_string));
		quoted_string = join_and_cleanup(&quoted_string, &subsequent_string);
		if (!quoted_string)
			return (return_mem_alloc_error());
	}
	return (quoted_string);
}

char	*extract_quoted_string(const char *input, size_t *pos,
			t_token_list **tokens, t_token_flags *flags)
{
	char		*tmp;
	const char	*start;

	start = input + *pos;
	if (!flags->var)
		flags->var = initialize_var_string(input, *pos, flags, start);
	(*pos)++;
	tmp = NULL;
	while (input[*pos] && input[*pos] != flags->quote_type)
	{
		if (flags->quote_type == '\"' && input[*pos] == '$')
			tmp = substitute_variable(input, pos, tokens, flags);
		else
		{
			tmp = ft_strndup(input + *pos, 1);
			(*pos)++;
		}
		if (!tmp)
		{
			free(flags->string);
			return (return_mem_alloc_error());
		}
		flags->string = join_and_cleanup(&flags->string, &tmp);
	}
	return (flags->string);
}

char	*substitute_variable(const char *input, size_t *pos,
			t_token_list **tokens, t_token_flags *flags)
{
	char	*var;

	var = ft_strdup("");
	if (input[*pos + 1] == '?')
	{
		if (flags->string && ft_strlen(flags->string) > 0)
		{
			if (add_token(tokens, TOKEN_STRING, flags->string) != SUCCESS)
				return (free_2_and_return_null(flags->string, var));
			free(flags->string);
			flags->string = ft_strdup("");
		}
		if (add_token(tokens, TOKEN_EXIT_CODE, "$?") != SUCCESS)
			return (free_2_and_return_null(flags->string, var));
		*pos += 2;
	}
	else
	{
		free(var);
		var = quotes_handle_variable_expansion(input, pos, flags);
	}
	return (var);
}

int	misuse_or_unclosed_quotes_error(t_token_list **tokens)
{
	if (*tokens)
	{
		if ((*tokens)->tail->token.type == TOKEN_ERROR)
			return (unclosed_quotes_error(tokens));
		return (MISUSE);
	}
	return (MISUSE);
}

int	unclosed_quotes_error(t_token_list **tokens)
{
	ft_dprintf(STDERR_FILENO, "%s\n", (*tokens)->tail->token.value);
	free_token_list(tokens);
	return (GENERAL_ERROR);
}
