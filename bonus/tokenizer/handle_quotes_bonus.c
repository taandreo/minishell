#include "minishell_bonus.h"

char			*advance_position(const char *input, size_t *pos,
					t_token_list **tokens, char quote_type);

char	*handle_quotes(const char *input, size_t *position,
			t_token_list **tokens)
{
	char	quote_type;
	size_t	start;
	char	*quoted_string;
	char	*subsequent_string;

	quote_type = input[*position];
	start = *position + 1;
	quoted_string = advance_position(input, position, tokens, quote_type);
	subsequent_string = NULL;
	if (input[*position] != quote_type)
	{
		add_token(tokens, TOKEN_ERROR, "minishell: Unclosed quotes");
		return (NULL);
	}
//	quoted_string = extract_quoted_string(input, start, *position);
	if (!quoted_string)
		return (return_mem_alloc_error());
	(*position)++;
	if (input[*position] && is_string_start(input[*position]))
	{
		subsequent_string = get_string_from_input(input, position, tokens);
		quoted_string = ft_strjoin(quoted_string, subsequent_string);
		if (!quoted_string)
			return (return_mem_alloc_error());
		free(subsequent_string);
	}
	return (quoted_string);
}

char	*advance_position(const char *input, size_t *pos,
			t_token_list **tokens, char quote_type)
{
	char	*buffer;
	char	*tmp;
	size_t	start;

	start = *pos + 1;
	(*pos)++;
	tmp = NULL;
	buffer = ft_strdup("");
	if (!buffer)
		return (return_mem_alloc_error());
	while (input[*pos] && input[*pos] != quote_type)
	{
		if (quote_type == '\"' && input[*pos] == '$')
			tmp = substitute_variable(input, pos, tokens, buffer);
		else
		{
			tmp = ft_strndup(input + *pos, 1);
			(*pos)++;
		}
		if (!tmp)
		{
			free(buffer);
			return (return_mem_alloc_error());
		}
		if (ft_strlen(buffer) == 0 && ft_strlen(tmp) == 0)
			break ;
		buffer = join_and_cleanup(&buffer, &tmp);
	}
	return (buffer);
}

char	*substitute_variable(const char *input, size_t *pos,
			t_token_list **tokens, char *buffer)
{
	if (input[*pos + 1] == '?')
	{
		if (buffer && ft_strlen(buffer) > 0)
		{
			if (add_token(tokens, TOKEN_STRING, buffer) != SUCCESS)
				return (free_and_return_null(buffer));
			free(buffer);
			buffer = ft_strdup("");
		}
		if (add_token(tokens, TOKEN_EXIT_CODE, "$?") != SUCCESS)
			return (free_and_return_null(buffer));
		*pos += 2;
	}
	if (ft_strlen(input + *pos + 1) == 0)
		return (ft_strdup(""));
	buffer = handle_variable_expansion(input, pos, buffer);
	return (buffer);
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
