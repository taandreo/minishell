#include "minishell_bonus.h"

static size_t	advance_position(const char *input, size_t position,
					char quote_type);
static char		*extract_quoted_string(const char *input, size_t start,
					size_t end);

char	*handle_quotes(const char *input, size_t *position,
			t_token_list **tokens)
{
	char	quote_type;
	size_t	start;
	char	*quoted_string;
	char	*subsequent_string;

	quote_type = input[*position];
	start = *position + 1;
	*position = advance_position(input, *position, quote_type);
	subsequent_string = NULL;
	if (input[*position] != quote_type)
	{
		add_token(tokens, TOKEN_ERROR, "minishell: Unclosed quotes");
		return (NULL);
	}
	quoted_string = extract_quoted_string(input, start, *position);
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

static size_t	advance_position(const char *input, size_t position,
					char quote_type)
{
	// TODO: maybe use start for variable expansion, keep for later.
//	size_t	start;
//
//	start = position + 1;
	position++;
	while (input[position] && input[position] != quote_type)
	{
//		if (quote_type == '\"' && input[position] == '$')
//		{
//			// TODO: Variable Expansion
//			position++;
//		}
		position++;
	}
	return (position);
}

static char	*extract_quoted_string(const char *input, size_t start,
					size_t end)
{
	size_t	length;

	length = end - start;
	return (ft_strndup(input + start, length));
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
