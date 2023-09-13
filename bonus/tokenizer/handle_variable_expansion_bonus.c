#include "minishell_bonus.h"

t_bool	add_space_if_success(size_t *pos, t_token_list **tokens,
			t_token_flags *flags);

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
		&& ((*tokens)->tail->token.type == TOKEN_STRING
		|| (*tokens)->tail->token.type == TOKEN_COMMAND_NAME
		|| is_builtin((*tokens)->tail->token.value)))
	{
		if (add_token(tokens, TOKEN_SPACE, " ") != SUCCESS)
			return (free_nullify_and_return_null(&flags->string) != NULL);
	}
	if (flags->string && ft_strlen(flags->string) > 0)
		if (!add_command_or_string(tokens, flags))
			return (false);
	if (add_token(tokens, TOKEN_EXIT_CODE, "$?") != SUCCESS)
		return (free_nullify_and_return_null(&flags->string) != NULL);
	free_str_and_nullify(&flags->string);
	*pos += 2;
	flags->has_exit_code = true;
	if (input[*pos] == ' ' && input[*pos + 1])
		return (add_space_if_success(pos, tokens, flags));
	return (true);
}

t_bool	add_space_if_success(size_t *pos, t_token_list **tokens,
		t_token_flags *flags)
{
	if (add_token(tokens, TOKEN_SPACE, " ") != SUCCESS)
	{
		flags->has_exit_code = false;
		return (free_nullify_and_return_null(&flags->string) != NULL);
	}
	(*pos)++;
	return (true);
}

t_bool	add_command_or_string(t_token_list **tokens, t_token_flags *flags)
{
	if (flags->is_command)
	{
		if (add_token(tokens, TOKEN_COMMAND_NAME, flags->string) != SUCCESS)
			return (free_nullify_and_return_null(&flags->string) != NULL);
		flags->is_command = false;
		flags->has_command = true;
	}
	else
	{
		if (add_token(tokens, TOKEN_STRING, flags->string) != SUCCESS)
			return (free_nullify_and_return_null(&flags->string) != NULL);
	}
	return (true);
}
