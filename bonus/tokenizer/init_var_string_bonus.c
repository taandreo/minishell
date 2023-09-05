#include "minishell_bonus.h"

void	*initialize_var_string(const char *input, size_t pos,
			t_token_flags *flags, const char *start)
{
	char	*tmp;

	while (input[pos] && is_string_start(input[pos]))
		pos++;
	tmp = ft_strndup(start, input + pos - start);
	if (!tmp)
		return (free_and_return_null(flags->string));
	flags->var = ft_strjoin(flags->string, tmp);
	free(tmp);
	return (flags->var);
}
