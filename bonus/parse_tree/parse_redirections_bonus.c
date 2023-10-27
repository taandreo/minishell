/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 19:40:35 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/15 19:40:36 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

t_bool	next_redirection_and_arguments(t_command_part *command_part,
		t_redirections *initial_redirections, t_token_list *tokens,
		t_parser_state *state)
{
	next_redirections(command_part, initial_redirections, tokens, state);
	if (state->error)
	{
		free_command_part(command_part);
		return (false);
	}
	if (!state->has_paren || (state->has_paren && state->paren_count > 0))
		subsequent_arguments(command_part, tokens, state);
	return (true);
}

t_bool	parse_next_red(t_command_part *command_part,
			t_redirections *initial_redirections, t_token_list *tokens,
			t_parser_state *state)
{
	while (is_redirection_or_string(current_token_type(tokens)))
	{
		if (!next_redirection_and_arguments(command_part, initial_redirections,
				tokens, state))
			return (false);
		initial_redirections = command_part->redirections;
	}
	return (true);
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

t_bool	is_redirection_or_string(t_token_type token)
{
	if ((token >= TOKEN_REDIRECTION_INPUT && token <= TOKEN_REDIRECTION_HEREDOC)
		|| token == TOKEN_STRING
		|| token == TOKEN_EXIT_CODE
		|| token == TOKEN_WILDCARD
		|| token == TOKEN_SPACE)
		return (true);
	return (false);
}
