#include "minishell_bonus.h"

t_redirections	*parse_redirections(t_token_list *tokens)
{
	t_redirections	*redirections;

	if (!(current_token_type(tokens) >= TOKEN_REDIRECTION_INPUT &&
		current_token_type(tokens) <= TOKEN_REDIRECTION_HEREDOC))
		return (NULL);
	redirections = malloc(sizeof(t_redirections));
	redirections->redirection->type = current_token_type(tokens);
	advance_token(tokens);
//	if (current_token_type(tokens) == TOKEN_FILENAME
//		|| current_token_type(tokens) == TOKEN_STRING)
//		redirections->redirection->filename = ft_strdup(current_token(tokens).value);
	return (redirections);
}