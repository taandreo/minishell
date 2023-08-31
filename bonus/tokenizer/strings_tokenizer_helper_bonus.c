#include "minishell_bonus.h"

t_bool	handle_character(const char *input, size_t *pos, t_token_list **tokens,
						   t_token_flags *flags);
char	*process_quotes(const char *input, size_t *pos, t_token_list **tokens,
						t_token_flags *flags);

char	*get_string_from_input(const char *input, size_t *pos,
		t_token_list **tokens, t_token_flags *flags)
{
	char		*tmp;
	const char	*start;
	size_t		init_pos;

	init_pos = *pos;
	flags->string = ft_strdup("");
	start = input + *pos;
	tmp = NULL;
	if (!flags->string)
		return (return_mem_alloc_error());
	while (input[*pos] && is_string_start(input[*pos]))
		(*pos)++;
	tmp = ft_strndup(start, input + *pos - start);
	if (!tmp)
	{
		free(flags->string);
		return (NULL);
	}
	flags->var = ft_strjoin(flags->string, tmp);
	*pos = init_pos;
	while (input[*pos] && is_string_start(input[*pos]))
	{
		if (handle_character(input, pos, tokens, flags))
			start = input + *pos;
		if (!flags->string)
			return (NULL);
	}
	tmp = ft_strndup(start, input + *pos - start);
	if (!tmp)
	{
		free(flags->string);
		return (NULL);
	}
	return (join_and_cleanup(&flags->string, &tmp));
}

char	*process_quotes(const char *input, size_t *pos, t_token_list **tokens,
			t_token_flags *flags)
{
	char		*tmp;
	const char	*start;

	start = input + *pos;
	tmp = ft_strndup(start, input + *pos - start);
	if (!tmp)
	{
		free(flags->string);
		return (NULL);
	}
	flags->string = join_and_cleanup(&flags->string, &tmp);
	if (!flags->string)
		return (NULL);
	tmp = handle_quotes(input, pos, tokens, flags);
	if (!tmp)
	{
		free(flags->string);
		return (NULL);
	}
	return (tmp);
}

t_bool	handle_character(const char *input, size_t *pos, t_token_list **tokens,
			t_token_flags *flags)
{
	char	*buffer;
	char	*expanded_var;

	buffer = NULL;
	if (input[*pos] == '\'' || input[*pos] == '\"')
	{
		flags->string = process_quotes(input, pos, tokens, flags);
		return (flags->string != NULL);
	}
	if (input[*pos] == '$' && !flags->has_heredoc)
	{
		if (input[*pos + 1] == '?')
		{
			if (*pos > 0 && ft_is_space(input[*pos - 1])
				&& (*tokens)->tail->token.type == TOKEN_STRING)
			{
				if (add_token(tokens, TOKEN_SPACE, " ") != SUCCESS)
					return (free_and_return_null(flags->string) != NULL);
			}
			if (flags->string && ft_strlen(flags->string) > 0)
			{
				if (add_token(tokens, TOKEN_STRING, flags->string) != SUCCESS)
					return (free_and_return_null(flags->string) != NULL);
				free(flags->string);
				flags->string = ft_strdup("");
			}
			if (add_token(tokens, TOKEN_EXIT_CODE, "$?") != SUCCESS)
				return (free_and_return_null(flags->string) != NULL);
			*pos += 2;
			if (input[*pos] == ' ')
			{
				if (add_token(tokens, TOKEN_SPACE, " ") != SUCCESS)
					return (free_and_return_null(flags->string) != NULL);
				(*pos)++;
			}
			return (true);
		}
		expanded_var = handle_variable_expansion(input, pos, flags);
		flags->string = join_and_cleanup(&flags->string, &expanded_var);
		return (flags->string != NULL);
	}
	buffer = ft_strndup(input + *pos, 1);
	if (!buffer)
		return (free_and_return_null(flags->string) != NULL);
	flags->string = join_and_cleanup(&flags->string, &buffer);
	(*pos)++;
	return (true);
}

char	*join_and_cleanup(char **malloced_str1, char **malloced_str2)
{
	char	*new_str;

	if (!malloced_str1 || !malloced_str2 || !*malloced_str1 || !*malloced_str2)
		return (NULL);
	new_str = ft_strjoin(*malloced_str1, *malloced_str2);
	free(*malloced_str1);
	*malloced_str1 = NULL;
	free(*malloced_str2);
	*malloced_str2 = NULL;
	if (!new_str)
		return (return_mem_alloc_error());
	return (new_str);
}

char	*handle_variable_expansion(const char *input, size_t *pos,
			t_token_flags *flags)
{
	char *tmp;

	tmp = expand_variable_string(input, pos);
	if (!tmp)
	{
		if (flags->string)
			free(flags->string);
		return (NULL);
	}
	return (tmp);
}
