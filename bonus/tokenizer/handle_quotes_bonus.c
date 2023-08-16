#include "minishell_bonus.h"

static size_t	advance_position(const char *input, size_t position,
					char quote_type);
static char		*extract_quoted_string(const char *input, size_t start,
					size_t end);

char	*handle_quotes(const char *input, size_t *position,
			t_token_list *tokens)
{
	char	quote_type;
	size_t	start;
	char	*quoted_string;

	quote_type = input[*position];
	start = *position + 1;
	*position = advance_position(input, *position, quote_type);
	if (input[*position] != quote_type)
	{
		add_token(tokens, TOKEN_ERROR, "Error: Unclosed quotes.");
		return (NULL);
	}
	quoted_string = extract_quoted_string(input, start, *position);
	if (!quoted_string)
	{
		add_token(tokens, TOKEN_ERROR, "Error: Unable to alloc memory");
		return (NULL);
	}
	(*position)++;
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
