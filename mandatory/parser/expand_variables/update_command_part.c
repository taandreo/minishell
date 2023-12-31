/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_command_part.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:46:38 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/28 21:42:32 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_name_quotes_routine(t_command_part *cmd_part, t_vars *vars);
int	cmd_name_unquotes_routine(t_command_part *cmd_part, t_vars *vars);
int	args_routine(t_command_part *cmd_part, t_vars *vars);
int	redirections_routine(t_command_part *cmd_part, t_vars *vars);

t_bool	update_cmd_part_values(t_command_part *cmd_part, t_vars *vars)
{
	vars->state.status = cmd_name_quotes_routine(cmd_part, vars);
	vars->state.status = cmd_name_unquotes_routine(cmd_part, vars);
	vars->state.status = args_routine(cmd_part, vars);
	vars->state.status = redirections_routine(cmd_part, vars);
	if (vars->state.error)
		return (false);
	return (true);
}

int	cmd_name_quotes_routine(t_command_part *cmd_part, t_vars *vars)
{
	if (vars->state.status != SUCCESS && vars->state.error == true)
		return (vars->state.status);
	if (cmd_part->type == TOKEN_COMMAND_NAME_QUOTES)
	{
		cmd_part->u_cmd.cmd_name = concat_string(
				cmd_part->u_cmd.cmd_name, vars,
				TOKEN_COMMAND_NAME_QUOTES);
		if (!cmd_part->u_cmd.cmd_name && vars->state.error == false)
		{
			cmd_part->u_cmd.builtin_cmd = malloc(sizeof(t_builtin_cmd));
			cmd_part->u_cmd.builtin_cmd->type = cmd_part->type;
			cmd_part->u_cmd.builtin_cmd->value = ft_strdup(
					builtin_type_to_value(cmd_part->type));
			if (!cmd_part->u_cmd.builtin_cmd->value)
			{
				print_mem_alloc_error();
				vars->state.status = MISUSE;
				vars->state.error = true;
				vars->state.is_set = true;
			}
		}
	}
	return (vars->state.status);
}

int	cmd_name_unquotes_routine(t_command_part *cmd_part, t_vars *vars)
{
	if (vars->state.status != SUCCESS && vars->state.error == true)
		return (vars->state.status);
	if (cmd_part->type == TOKEN_COMMAND_NAME_UNQUOTES)
	{
		cmd_part->u_cmd.cmd_name = concat_string(
				cmd_part->u_cmd.cmd_name, vars,
				TOKEN_COMMAND_NAME_UNQUOTES);
		if (!cmd_part->u_cmd.cmd_name && vars->state.error == false)
		{
			cmd_part->u_cmd.builtin_cmd = malloc(sizeof(t_builtin_cmd));
			cmd_part->u_cmd.builtin_cmd->type = cmd_part->type;
			cmd_part->u_cmd.builtin_cmd->value = ft_strdup(
					builtin_type_to_value(cmd_part->type));
			if (!cmd_part->u_cmd.builtin_cmd->value)
			{
				print_mem_alloc_error();
				vars->state.status = MISUSE;
				vars->state.error = true;
				vars->state.is_set = true;
			}
		}
	}
	return (vars->state.status);
}

int	args_routine(t_command_part *cmd_part, t_vars *vars)
{
	t_arguments		*current_args;

	if (vars->state.status != SUCCESS && vars->state.error)
		return (vars->state.status);
	current_args = cmd_part->arguments;
	while (current_args)
	{
		current_args->type = TOKEN_STRING;
		current_args->string = concat_string(current_args->string, vars,
				TOKEN_STRING);
		if (vars->state.status != SUCCESS && vars->state.error == true)
			break ;
		current_args = current_args->next;
	}
	return (vars->state.status);
}

int	redirections_routine(t_command_part *cmd_part, t_vars *vars)
{
	t_redirections	*current_redir;

	if (vars->state.status != SUCCESS && vars->state.error == true)
		return (vars->state.status);
	if (cmd_part->redirections && cmd_part->redirections->redirection)
	{
		current_redir = cmd_part->redirections;
		while (current_redir)
		{
			current_redir->redirection->filename = concat_string(
					current_redir->redirection->filename,
					vars, TOKEN_FILENAME);
			if (vars->state.status != SUCCESS && vars->state.error == true)
				break ;
			current_redir = current_redir->next;
		}
	}
	return (vars->state.status);
}
