#include "minishell_bonus.h"

void	*null_and_maybe_print_misuse(t_redirections *redir,
		t_parser_state *state);

t_redirections	*parse_redirections(t_token_list *tokens, t_parser_state *state)
{
	t_redirections	*redir;

	if (!(current_token_type(tokens) >= TOKEN_REDIRECTION_INPUT
			&& current_token_type(tokens) <= TOKEN_REDIRECTION_HEREDOC))
		return (NULL);
	redir = malloc(sizeof(t_redirections));
	if (!redir)
		return (print_misuse_state_error(state));
	redir->redirection = malloc(sizeof(t_redirection));
	redir->next = NULL;
	if (!redir->redirection)
	{
		free_redirections(redir);
		return (print_misuse_state_error(state));
	}
	redir->redirection->type = current_token_type(tokens);
	advance_token(tokens);
	redir->redirection->filename = parse_string(tokens, state);
	if (!redir->redirection->filename)
		return (null_and_maybe_print_misuse(redir, state));
	redir->next = parse_redirections(tokens, state);
	return (redir);
}

void	*null_and_maybe_print_misuse(t_redirections *redir,
			t_parser_state *state)
{
	free_redirections(redir);
	state->error = true;
	if (state->status != MISUSE)
		return (NULL);
	return (print_misuse_state_error(state));
}
