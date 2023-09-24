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
						t_token_list *tokens)
{
	command_part->type = current_token_type(tokens);
	command_part->u_cmd.builtin_cmd = malloc(sizeof(t_builtin_cmd));
	if (!command_part->u_cmd.builtin_cmd)
	{
		free_command_part(command_part);
		return (return_mem_alloc_error());
	}
	command_part->u_cmd.builtin_cmd->type = command_part->type;
	command_part->u_cmd.builtin_cmd->value = ft_strdup(current_token(tokens).value);
	if (!command_part->u_cmd.builtin_cmd->value)
	{
		free_command_part(command_part);
		return (return_mem_alloc_error());
	}
	advance_token(tokens);
	command_part->arguments = parse_arguments(tokens);
	return (command_part->u_cmd.builtin_cmd);
}

t_string	*handle_command_name_tokens(t_command_part *command_part,
					t_token_list *tokens)
{
	command_part->type = TOKEN_COMMAND_NAME;
	command_part->u_cmd.cmd_name = parse_string(tokens);
	if (!command_part->u_cmd.cmd_name)
	{
		free_command_part(command_part);
		return (return_mem_alloc_error());
	}
	if (current_token_type(tokens) == TOKEN_SPACE)
		advance_token(tokens);
	if (current_token_type(tokens) == TOKEN_END)
		command_part->arguments = NULL;
	else
		command_part->arguments = parse_arguments(tokens);
	return (command_part->u_cmd.cmd_name);
}

t_bool	is_operator_or_end(t_token_type type)
{
	if ((type >= TOKEN_AND && type <= TOKEN_PIPE)
			|| (type >= TOKEN_REDIRECTIONS && type <= TOKEN_REDIRECTION_HEREDOC)
			|| type == TOKEN_END)
		return (true);
	return (false);
}
