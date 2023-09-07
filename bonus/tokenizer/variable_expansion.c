#include "minishell_bonus.h"

t_bool	is_valid_var_name(char c);
char	*lookup_variable(char *var);
char	*advance_position(const char *input, size_t *pos);

char	*expand_variable_string(const char *input, size_t *pos)
{
	size_t	start_pos;
	size_t	len;
	char	*var;
	char	*str;

	len = 0;
	start_pos = *pos + 1;
	var = ft_strdup("");
	str = NULL;
	if (!ft_isalpha(input[start_pos]) && input[start_pos] != '_')
		return (advance_position(input, pos));
	while (input[start_pos + len] && is_valid_var_name(input[start_pos + len]))
		len++;
	free(var);
	var = ft_strndup(input + start_pos, len);
	if (!var)
		return (return_mem_alloc_error());
	str = lookup_variable(var);
	free(var);
	*pos = start_pos + len;
	return (str);
}

t_bool	is_valid_var_name(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*lookup_variable(char *var)
{
	char	*value;
	char	*new_str;

	new_str = NULL;
	value = getenv(var);
	if (!value)
		return (ft_strdup(""));
	new_str = ft_strdup(value);
	if (!new_str)
		return (return_mem_alloc_error());
	return (new_str);
}

char	*advance_position(const char *input, size_t *pos)
{
	char	*var;

	while (is_string_start(input[*pos]))
		(*pos)++;
	var = ft_strdup("");
	if (!var)
		return (return_mem_alloc_error());
	return (var);
}

char	*quotes_handle_variable_expansion(const char *input, size_t *pos,
		t_token_flags *flags)
{
	char	*tmp;

	flags->inside_quotes = true;
	tmp = expand_variable_string(input, pos);
	if (!tmp)
	{
		if (flags->string)
			free(flags->string);
		return (NULL);
	}
	return (tmp);
}
