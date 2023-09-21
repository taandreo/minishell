#include "minishell_bonus.h"

t_filename	*add_filename_list(t_token_list *tokens);

t_redirections	*parse_redirections(t_token_list *tokens)
{
	t_redirections	*redir;

	if (!(current_token_type(tokens) >= TOKEN_REDIRECTION_INPUT &&
		current_token_type(tokens) <= TOKEN_REDIRECTION_HEREDOC))
		return (NULL);
	redir = malloc(sizeof(t_redirections));
	redir->redirection->type = current_token_type(tokens);
	advance_token(tokens);
	redir->redirection->filename = add_filename_list(tokens);

	return (redir);
}

t_filename	*add_filename_list(t_token_list *tokens)
{
	t_filename *filename;

	filename = NULL;
	if (current_token_type(tokens) == TOKEN_FILENAME
			|| current_token_type(tokens) == TOKEN_STRING)
	{
		filename->type = current_token_type(tokens);
		filename->value = ft_strdup(current_token(tokens).value);
	}
	else if (current_token_type(tokens) == TOKEN_EXIT_CODE
			|| current_token_type(tokens) == TOKEN_WILDCARD)
	{
		filename->type = current_token_type(tokens);
		filename->value = ft_strdup(current_token(tokens).value);
		if (peek_token(tokens).type != TOKEN_SPACE)
		{
			advance_token(tokens);
			filename->next = add_filename_list(tokens);
		}
	}
	return (filename);
}