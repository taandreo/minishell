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

	flags->string = ft_strdup("");
	start = input + *pos;
	tmp = NULL;
	if (!flags->string)
		return (return_mem_alloc_error());
	if (!flags->var)
		flags->var = initialize_var_string(input, *pos, flags, start);
	while (input[*pos] && is_string_start(input[*pos]))
	{
		if (handle_character(input, pos, tokens, flags))
			start = input + *pos;
		if (!flags->string)
			return (NULL);
	}
	tmp = ft_strndup(start, input + *pos - start);
	if (!tmp)
		return (free_and_return_null(flags->string));
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
		return (free_and_return_null(flags->string));
	flags->string = join_and_cleanup(&flags->string, &tmp);
	if (!flags->string)
		return (NULL);
	tmp = handle_quotes(input, pos, tokens, flags);
	if (!tmp)
		return (free_and_return_null(flags->string));
	return (tmp);
}

t_bool	handle_character(const char *input, size_t *pos, t_token_list **tokens,
			t_token_flags *flags)
{
	char	*buffer;

	buffer = NULL;
	if (input[*pos] == '\'' || input[*pos] == '\"')
	{
		flags->string = process_quotes(input, pos, tokens, flags);
		return (flags->string != NULL);
	}
	if (input[*pos] == '$' && !flags->has_heredoc)
		return (has_variable_expansion(input, pos, tokens, flags));
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

char	*strings_handle_variable_expansion(const char *input, size_t *pos,
			t_token_flags *flags)
{
	char	*tmp;

	flags->inside_quotes = false;
	tmp = expand_variable_string(input, pos);
	if (!tmp)
	{
		if (flags->string)
			free(flags->string);
		return (NULL);
	}
	return (tmp);
}
