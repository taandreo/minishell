#include "minishell_bonus.h"

t_redirections	*parse_redirections(t_token_list *tokens, t_parser_state *state)
{
	t_redirections	*redir;

	if (!(current_token_type(tokens) >= TOKEN_REDIRECTION_INPUT &&
		current_token_type(tokens) <= TOKEN_REDIRECTION_HEREDOC))
		return (NULL);
	redir = malloc(sizeof(t_redirections));
	if (!redir)
	{
		state->error = true;
		return (return_mem_alloc_error());
	}
	redir->redirection = malloc(sizeof(t_redirection));
	redir->next = NULL;
	if (!redir->redirection)
	{
		free_redirections(redir);
		state->error = true;
		return (return_mem_alloc_error());
	}
	redir->redirection->type = current_token_type(tokens);
	advance_token(tokens);
	redir->redirection->filename = parse_string(tokens);
	if (!redir->redirection->filename)
	{
		free_redirections(redir);
		state->error = true;
		return (return_syntax_error(current_token(tokens).value));
	}
	redir->next = parse_redirections(tokens, state);
	return (redir);
}
