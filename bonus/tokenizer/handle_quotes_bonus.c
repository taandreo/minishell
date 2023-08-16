#include "minishell_bonus.h"

char	*handle_quotes(const char *input, size_t *position,
			t_token_list *tokens)
{
	char	quote_type;
	size_t	start;
	size_t	length;
	char	*quoted_string;

	quote_type = input[*position];
	start = *position + 1;
	(*position)++;
	while (input[*position] && input[*position] != quote_type)
	{
		if (quote_type == '\"' && input[*position] == '$')
		{
			// TODO: Variable Expansion
			(*position)++;
		}
		(*position)++;
	}
	if (input[*position] != quote_type)
	{
		add_token(tokens, TOKEN_ERROR, "Error: Unclosed quotes.");
		return (NULL);
	}
	length = *position - start;
	quoted_string = ft_strndup(input + start, length);
	if (!quoted_string)
	{
		add_token(tokens, TOKEN_ERROR, "Error: Unable to alloc memory");
		return (NULL);
	}
	(*position)++;
	return (quoted_string);
}
