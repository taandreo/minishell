#include "minishell_bonus.h"

char	*extract_quoted_string(char **input, size_t *pos,
			t_token_list **tokens, t_token_flags *flags);

char	*handle_quotes(char **input, size_t *position,
			t_token_list **tokens, t_token_flags *flags)
{
	flags->quote_type = (*input)[*position];
	if (!flags->string)
	{
		flags->string = ft_strdup("");
		if (!flags->string)
			return (return_mem_alloc_error());
	}
	flags->string = extract_quoted_string(input, position, tokens, flags);
	if (!flags->string && !flags->has_exit_code)
		return (return_mem_alloc_error());
	if ((*input)[*position] != flags->quote_type)
		return (add_unclosed_quotes_token(tokens, flags, flags->string));
	(*position)++;
	flags->init_var = true;
	if ((*input)[*position] && is_string_start((*input)[*position], flags))
	{
		flags->init_var = false;
		return (get_string_from_input(input, position, tokens, flags));
	}
	return (flags->string);
}

char	*extract_quoted_string(char **input, size_t *pos,
			t_token_list **tokens, t_token_flags *flags)
{
	char *tmp;
	const char *start;

	start = *input + *pos;
	if (!flags->var)
	{
		flags->var = initialize_var_string(*input, *pos, flags, start);
		if (!flags->var)
			return (NULL);
	}
	(*pos)++;
	while ((*input)[*pos] && (*input)[*pos] != flags->quote_type)
	{
		if (flags->quote_type == '\"' && (*input)[*pos] == '$')
			tmp = substitute_variable(*input, pos, tokens, flags);
		else
		{
			tmp = ft_strndup(*input + *pos, 1);
			(*pos)++;
		}
		if (!tmp)
		{
			free_str_and_nullify(&flags->string);
			return (return_mem_alloc_error());
		}
		flags->string = join_and_cleanup(&flags->string, &tmp);
	}
	if (flags->has_exit_code && flags->string && ft_strlen(flags->string) == 0)
		free_str_and_nullify(&flags->string);
	return (flags->string);
}

char	*substitute_variable(char *input, size_t *pos,
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
		else if (flags->string && ft_strlen(flags->string) == 0)
		{
			if (*pos - 2 > 0 && input[*pos - 2] == ' ')
			{
				if (add_token(tokens, TOKEN_SPACE, " ") != SUCCESS)
					return (free_2_and_return_null(flags->string, var));
			}

		}
		if (add_token(tokens, TOKEN_EXIT_CODE, "$?") != SUCCESS)
			return (free_2_and_return_null(flags->string, var));
		*pos += 2;
		flags->has_exit_code = true;
	}
	else
	{
		free(var);
		var = quotes_handle_variable_expansion(&input, pos, flags);
	}
	return (var);
}
