#include "minishell_bonus.h"


t_bool	handle_character(const char *input, size_t *pos, t_token_list **tokens,
						   char **return_string);
char	*process_quotes(const char *input, size_t *pos, t_token_list **tokens,
						char *return_string);

char	*get_string_from_input(const char *input, size_t *pos,
							   t_token_list **tokens)
{
	char		*return_string;
	char		*tmp;
	const char	*start;

	start = input + *pos;
	return_string = ft_strdup("");
	tmp = NULL;
	if (!return_string)
		return (return_mem_alloc_error());
	while (input[*pos] && is_string_start(input[*pos]))
	{
		if (handle_character(input, pos, tokens, &return_string))
			start = input + *pos;
		if (!return_string)
			return (NULL);
	}
	tmp = ft_strndup(start, input + *pos - start);
	if (!tmp)
	{
		free(return_string);
		return (NULL);
	}
	if (ft_strlen(tmp) == 0 && ft_strlen(return_string) == 0)
		return (free_2_and_return_null(return_string, tmp));
	return (join_and_cleanup(&return_string, &tmp));
}

char	*process_quotes(const char *input, size_t *pos, t_token_list **tokens,
			char *return_string)
{
	char		*tmp;
	const char	*start;

	start = input + *pos;
	tmp = ft_strndup(start, input + *pos - start);
	if (!tmp)
	{
		free(return_string);
		return (NULL);
	}
	return_string = join_and_cleanup(&return_string, &tmp);
	if (!return_string)
		return (NULL);
	tmp = handle_quotes(input, pos, tokens);
	if (!tmp)
	{
		free(return_string);
		return (NULL);
	}
	return (join_and_cleanup(&return_string, &tmp));
}

t_bool	handle_character(const char *input, size_t *pos, t_token_list **tokens,
			char **return_string)
{
	char	*buffer;

	buffer = NULL;
	if (input[*pos] == '\'' || input[*pos] == '\"')
	{
		*return_string = process_quotes(input, pos, tokens, *return_string);
		return (*return_string != NULL);
	}
	if (input[*pos] == '$')
	{
		if (input[*pos + 1] == '?')
		{
			if (*pos > 0 && ft_is_space(input[*pos - 1])
				&& (*tokens)->tail->token.type == TOKEN_STRING)
			{
				if (add_token(tokens, TOKEN_SPACE, " ") != SUCCESS)
					return (free_and_return_null(*return_string) != NULL);
			}
			if (*return_string && ft_strlen(*return_string) > 0)
			{
				if (add_token(tokens, TOKEN_STRING, *return_string) != SUCCESS)
					return (free_and_return_null(*return_string) != NULL);
				free(*return_string);
				*return_string = ft_strdup("");
			}
			if (add_token(tokens, TOKEN_EXIT_CODE, "$?") != SUCCESS)
				return (free_and_return_null(*return_string) != NULL);
			*pos += 2;
			if (input[*pos] == ' ')
			{
				if (add_token(tokens, TOKEN_SPACE, " ") != SUCCESS)
					return (free_and_return_null(*return_string) != NULL);
				(*pos)++;
			}
			return (true);
		}
		*return_string = handle_variable_expansion(input, pos, *return_string);
		return (*return_string != NULL);
	}
	buffer = ft_strndup(input + *pos, 1);
	if (!buffer)
		return (free_and_return_null(*return_string) != NULL);
	*return_string = join_and_cleanup(return_string, &buffer);
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

char	*handle_variable_expansion(const char *input, size_t *pos, char *current_str)
{
	char	*tmp;

	tmp = expand_variable_in_string(input, pos);
	if (!tmp)
	{
		free(current_str);
		return (NULL);
	}
	return (join_and_cleanup(&current_str, &tmp));
}
