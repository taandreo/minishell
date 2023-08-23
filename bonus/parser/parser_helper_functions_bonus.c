#include "minishell_bonus.h"

t_token	current_token(const t_token_list *tokens)
{
	t_token	empty_token;

	if (tokens->current)
		return (tokens->current->token);
	else
	{
		empty_token.type = TOKEN_END;
		empty_token.value = NULL;
		return (empty_token);
	}
}

t_token_type	current_token_type(t_token_list *tokens)
{
	return (current_token(tokens).type);
}

void	advance_token(t_token_list *tokens)
{
	if (tokens->current && tokens->current->next)
		tokens->current = tokens->current->next;
}

void	*free_and_return_null(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}
