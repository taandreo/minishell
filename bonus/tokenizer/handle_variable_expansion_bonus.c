#include "minishell_bonus.h"

t_bool	has_variable_expansion(char **input, size_t *pos,
		t_token_list **tokens, t_token_flags *flags)
{
	if ((*input)[*pos + 1] == '?')
		return (handle_special_case_variable(*input, pos, tokens, flags));
	if (strings_handle_variable_expansion(input, pos, flags) != SUCCESS)
		return (false);
	return (flags->string != NULL);
}

t_bool	handle_special_case_variable(char *input, size_t *pos,
		t_token_list **tokens, t_token_flags *flags)
{
	if (*pos > 0 && ft_is_space(input[*pos - 1])
		&& (*tokens)->tail->token.type == TOKEN_STRING)
	{
		if (add_token(tokens, TOKEN_SPACE, " ") != SUCCESS)
			return (free_nullify_and_return_null(&flags->string) != NULL);
	}
	if (flags->string && ft_strlen(flags->string) > 0)
	{
		if (add_token(tokens, TOKEN_STRING, flags->string) != SUCCESS)
			return (free_nullify_and_return_null(&flags->string) != NULL);
		free(flags->string);
		flags->string = ft_strdup("");
	}
	if (add_token(tokens, TOKEN_EXIT_CODE, "$?") != SUCCESS)
		return (free_nullify_and_return_null(&flags->string) != NULL);
	*pos += 2;
	if (input[*pos] == ' ')
	{
		if (add_token(tokens, TOKEN_SPACE, " ") != SUCCESS)
			return (free_nullify_and_return_null(&flags->string) != NULL);
		(*pos)++;
	}
	return (true);
}
