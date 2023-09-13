#include "minishell_bonus.h"

t_bool	handle_character(char **input, size_t *pos, t_token_list **tokens,
			t_token_flags *flags);
char	*process_quotes(char **input, size_t *pos, t_token_list **tokens,
			t_token_flags *flags);

t_bool	decrease_len(t_token_flags *flags)
{
	if (flags->var_len > -1)
		flags->var_len--;
	return (true);
}

char	*get_string_from_input(char **input, size_t *pos,
			t_token_list **tokens, t_token_flags *flags)
{
	const char	*start;

	if (!flags->string)
		flags->string = ft_strdup("");
	start = (*input) + *pos;
	if (!flags->string)
		return (return_mem_alloc_error());
	if (!flags->var)
		flags->var = initialize_var_string((*input), *pos, flags, start);
	while (decrease_len(flags) && (*input)[*pos]
			&& is_string_start((*input)[*pos], flags))
	{
		if (!handle_character(input, pos, tokens, flags))
			return (NULL);
		if (!flags->string)
			return (NULL);
	}
	return (flags->string);
}

char	*process_quotes(char **input, size_t *pos, t_token_list **tokens,
			t_token_flags *flags)
{
	char		*tmp;
	const char	*start;

	start = *input + *pos;
	tmp = ft_strndup(start, *input + *pos - start);
	if (!tmp)
		return (free_and_return_null(flags->string));
	flags->string = join_and_cleanup(&flags->string, &tmp);
	tmp = handle_quotes(input, pos, tokens, flags);
	if (!tmp)
		return (free_and_return_null(flags->string));
	return (tmp);
}

t_bool	handle_character(char **input, size_t *pos, t_token_list **tokens,
			t_token_flags *flags)
{
	char	*buffer;

	buffer = NULL;
	if (flags->var_len < 0)
	{
		if ((*input)[*pos] == '\'' || (*input)[*pos] == '\"')
		{
			flags->string = process_quotes(input, pos, tokens, flags);
			return (flags->string != NULL);
		}
		if ((*input)[*pos] == '$' && !flags->has_heredoc)
			return (has_variable_expansion(input, pos, tokens, flags));
	}
	buffer = ft_strndup((*input) + *pos, 1);
	if (!buffer)
	{
		free_str_and_nullify(&flags->string);
		return (flags->string != NULL);
	}
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

int	strings_handle_variable_expansion(char **input, size_t *pos,
			t_token_flags *flags)
{
	char	*tmp;
	char	*to_be_tokenized;
	char	*start;

	flags->inside_quotes = false;
	tmp = expand_variable_string(*input, pos, flags);
	if (!tmp)
	{
		if (flags->string)
			free(flags->string);
		return (MISUSE);
	}
	flags->var_len = ft_strlen(tmp);
	start = *input + *pos;
	to_be_tokenized = ft_strjoin(tmp, start);
	if (!to_be_tokenized)
	{
		if (flags->string)
			free(flags->string);
		free(tmp);
		return (MISUSE);
	}
	free(*input);
	free(tmp);
	*input = to_be_tokenized;
	*pos = 0;
	return (SUCCESS);
}
