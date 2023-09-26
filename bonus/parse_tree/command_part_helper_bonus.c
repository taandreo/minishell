/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_part_helper_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:32:43 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:32:44 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	subsequent_redirections(t_command_part *command_part,
			t_redirections *initial_redirections, t_token_list *tokens,
			t_parser_state *state)
{
	t_redirections	*subsequent_redirections;
	t_redirections	*last_initial_redirection;

	subsequent_redirections = parse_redirections(tokens, state);
	last_initial_redirection = initial_redirections;
	if (initial_redirections)
	{
		while (last_initial_redirection->next)
			last_initial_redirection = last_initial_redirection->next;
		last_initial_redirection->next = subsequent_redirections;
		command_part->redirections = initial_redirections;
	}
	else
		command_part->redirections = subsequent_redirections;
}

t_builtin_cmd	*handle_builtin_tokens(t_command_part *command_part,
						t_token_list *tokens, t_parser_state *state)
{
	command_part->type = current_token_type(tokens);
	command_part->u_cmd.builtin_cmd = malloc(sizeof(t_builtin_cmd));
	if (!command_part->u_cmd.builtin_cmd)
	{
		free_command_part(command_part);
		return (print_misuse_state_error(state));
	}
	command_part->u_cmd.builtin_cmd->type = command_part->type;
	command_part->u_cmd.builtin_cmd->value = ft_strdup(
			current_token(tokens).value);
	if (!command_part->u_cmd.builtin_cmd->value)
	{
		free_command_part(command_part);
		return (print_misuse_state_error(state));
	}
	advance_token(tokens);
	command_part->arguments = parse_arguments(tokens, state);
	return (command_part->u_cmd.builtin_cmd);
}

t_string	*handle_command_name_tokens(t_command_part *command_part,
					t_token_list *tokens, t_parser_state *state)
{
	command_part->type = TOKEN_COMMAND_NAME;
	command_part->u_cmd.cmd_name = parse_string(tokens, state);
	if (!command_part->u_cmd.cmd_name)
	{
		free_command_part(command_part);
		return (print_misuse_state_error(state));
	}
	if (current_token_type(tokens) == TOKEN_SPACE)
		advance_token(tokens);
	if (current_token_type(tokens) == TOKEN_END)
		command_part->arguments = NULL;
	else
		command_part->arguments = parse_arguments(tokens, state);
	return (command_part->u_cmd.cmd_name);
}

t_bool	add_command_union(t_command_part *command_part,
		t_redirections *redirections, t_token_list *tokens,
		t_parser_state *state)
{
	if (current_token_type(tokens) == TOKEN_LEFT_PARENTHESIS)
	{
		state->has_paren = true;
		command_part->u_cmd.grouping = parse_grouping(command_part, tokens,
				state);
	}
	else if (is_builtin_token(current_token_type(tokens)))
		command_part->u_cmd.builtin_cmd = handle_builtin_tokens(
				command_part, tokens, state);
	else if (current_token_type(tokens) == TOKEN_COMMAND_NAME)
		command_part->u_cmd.cmd_name = handle_command_name_tokens(
				command_part, tokens, state);
	else if (redirections)
	{
		command_part->type = TOKEN_REDIRECTIONS;
		command_part->redirections = redirections;
	}
	else
	{
		free_command_part(command_part);
		state->error = true;
		return (false);
	}
	return (true);
}
