#include "minishell_bonus.h"

t_bool	is_valid_var_name(char c);
char	*lookup_variable(char *var);

char	*expand_variable_in_string(const char *input, size_t *pos)
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
		return (var);
	while(input[start_pos + len] && is_valid_var_name(input[start_pos + len]))
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

t_bool is_valid_var_name(char c)
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