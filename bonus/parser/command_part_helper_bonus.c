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

void	add_subsequent_redirections_to_initial(t_command_part *command_part,
			t_redirections *initial_redirections, t_token_list *tokens)
{
	t_redirections	*subsequent_redirections;
	t_redirections	*last_initial_redirection;

	subsequent_redirections = parse_redirections(tokens);
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
	advance_token(tokens);
	command_part->u_cmd.builtin_cmd->arguments = parse_arguments(tokens);
	return (command_part->u_cmd.builtin_cmd);
}

t_string	*handle_command_name_tokens(t_command_part *command_part,
					t_token_list *tokens)
{
	command_part->type = TOKEN_COMMAND_NAME;
	command_part->u_cmd.cmd_name = malloc(sizeof(t_string));
	if (!command_part->u_cmd.cmd_name)
	{
		free_command_part(command_part);
		return (return_mem_alloc_error());
	}
	command_part->u_cmd.cmd_name->type = current_token_type(tokens);
	command_part->u_cmd.cmd_name->value = ft_strdup(current_token(tokens).value);
	if (!command_part->u_cmd.cmd_name->value)
	{
		free_command_part(command_part);
		return (return_mem_alloc_error());
	}
	advance_token(tokens);
	if (current_token_type(tokens) != TOKEN_SPACE && current_token_type(tokens) != TOKEN_END)
		command_part->u_cmd.cmd_name->next = handle_command_name_tokens(command_part, tokens);
	else
		command_part->arguments = parse_arguments(tokens);
	return (command_part->u_cmd.cmd_name);
}


