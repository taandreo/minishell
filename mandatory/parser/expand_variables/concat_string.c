/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_string_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:55:23 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/15 23:56:07 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

t_string	*concat_str_routine(t_string *string, t_command_part *cmd_part,
				t_vars *vars, t_token_type type);
t_string	*concat_unmatched_string(t_string *string, t_vars *vars);
t_string	*check_if_is_builtin(t_string *string, t_string *concat_str,
				t_command_part *cmd_part);

t_string	*concat_string(t_string *string, t_command_part *cmd_part,
		t_vars *vars, t_token_type type)
{
	string = expand_exit_code(string, vars, type);
	if (!string)
		return (NULL);
	string = concat_exit_code(string, vars, type);
	if (!string)
		return (NULL);
	if (is_token_cmd_name(cmd_part->type)
		|| (cmd_part->arguments && cmd_part->arguments->type == TOKEN_STRING)
		|| (cmd_part->redirections && cmd_part->redirections->redirection))
	{
		if (string->next || string->type == TOKEN_WILDCARD)
			string = concat_str_routine(string, cmd_part, vars, type);
	}
	return (string);
}

t_string	*concat_str_routine(t_string *string, t_command_part *cmd_part,
				t_vars *vars, t_token_type type)
{
	t_string		*concat_str;

	concat_str = expand_wildcard(string, vars, type);
	if (concat_str && concat_str->next)
	{
		if (cmd_part->redirections && cmd_part->redirections->redirection)
		{
			if (type == TOKEN_FILENAME)
				return (general_error_ambiguous_redirect(vars, string));
		}
		add_concat_string(cmd_part, concat_str, vars, type);
	}
	else if (concat_str)
	{
		free_string(string);
		string = NULL;
		string = check_if_is_builtin(string, concat_str, cmd_part);
	}
	else if (vars->state.status == SUCCESS)
		string = concat_unmatched_string(string, vars);
	else
		return (NULL);
	return (string);
}

t_string	*check_if_is_builtin(t_string *string, t_string *concat_str,
		t_command_part *cmd_part)
{
	if (is_builtin(concat_str->value))
	{
		cmd_part->type = get_builtin_token(concat_str->value);
		free_string(concat_str);
	}
	else
		string = concat_str;
	return (string);
}

t_string	*concat_unmatched_string(t_string *string, t_vars *vars)
{
	t_string	*current;

	current = string;
	while (current)
	{
		if (current->value)
		{
			while (current->next)
			{
				current->value = join_and_cleanup(&current->value,
						&current->next->value);
				if (!current->value)
				{
					free_string(string);
					vars->state.status = MISUSE;
					vars->state.error = true;
					return (return_mem_alloc_error());
				}
				current->next = clean_next_token(current);
			}
		}
		current = current->next;
	}
	return (string);
}
