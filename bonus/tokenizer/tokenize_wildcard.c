#include "minishell_bonus.h"

int	tokenize_wildcard(char *input, size_t *pos, t_token_list **tokens)
{
	int exit_status;

	exit_status = add_token(tokens, TOKEN_WILDCARD, "*");
	if (exit_status != SUCCESS)
		return (exit_status);
	(*pos)++;
	if (input[*pos] && ft_is_space(input[*pos]))
		exit_status = add_token(tokens, TOKEN_SPACE, " ");
	return (exit_status);
}
