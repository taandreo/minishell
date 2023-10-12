#include "minishell_bonus.h"

void	update_builtin(t_command_part  *cmd_part, t_string *concat_str,
			t_vars *vars);
void	update_cmd_name(t_command_part  *cmd_part, t_string *concat_str,
		t_vars *vars);
void	update_redirections(t_command_part  *cmd_part, t_string *concat_str,
		t_vars *vars);
void	update_arguments(t_command_part  *cmd_part, t_string *concat_str,
		t_vars *vars);

void	add_concat_string(t_command_part *cmd_part, t_string *concat_str,
		t_vars *vars, t_token_type type)
{
	if (type == TOKEN_COMMAND_NAME)
	{
		if (is_builtin(concat_str->value))
			update_builtin(cmd_part, concat_str, vars);
		else
			update_cmd_name(cmd_part, concat_str, vars);
	}
	else if (type == TOKEN_STRING)
		update_arguments(cmd_part, concat_str, vars);
	else if (type == TOKEN_FILENAME)
		update_redirections(cmd_part, concat_str, vars);
}

void	update_builtin(t_command_part  *cmd_part, t_string *concat_str,
			t_vars *vars)
{
	free_string(cmd_part->u_cmd.cmd_name);
	cmd_part->u_cmd.cmd_name = NULL;
	cmd_part->type = get_builtin_token(concat_str->value);
	cmd_part->u_cmd.builtin_cmd = malloc(sizeof(t_builtin_cmd));
	if (!cmd_part->u_cmd.builtin_cmd)
	{
		print_mem_alloc_error();
		vars->state.status = MISUSE;
		return ;
	}
	cmd_part->u_cmd.builtin_cmd->type = cmd_part->type;
	cmd_part->u_cmd.builtin_cmd->value = ft_strdup(
			builtin_type_to_value(cmd_part->type));
	if (!cmd_part->u_cmd.builtin_cmd->value)
	{
		print_mem_alloc_error();
		vars->state.status = MISUSE;
		return ;
	}
	concat_str = delete_first_node(concat_str);
	cmd_part->arguments = add_first_args(concat_str, cmd_part->arguments,
			vars, TOKEN_STRING);
}

void	update_cmd_name(t_command_part  *cmd_part, t_string *concat_str,
		t_vars *vars)
{
	free_string(cmd_part->u_cmd.cmd_name->next);
	cmd_part->u_cmd.cmd_name->next = NULL;
	cmd_part->u_cmd.cmd_name->value = ft_strdup(concat_str->value);
	if (!cmd_part->u_cmd.cmd_name->value)
	{
		print_mem_alloc_error();
		vars->state.status = MISUSE;
		return ;
	}
	concat_str = delete_first_node(concat_str);
	cmd_part->arguments = add_first_args(concat_str, cmd_part->arguments, vars,
			TOKEN_STRING);
}

void	update_redirections(t_command_part  *cmd_part, t_string *concat_str,
		t_vars *vars)
{
	t_redirection	*redir;

	redir = cmd_part->redirections->redirection;
	free_string(redir->filename->next);
	free(redir->filename->value);
	redir->filename->next = NULL;
	redir->filename->value = ft_strdup(concat_str->value);
	if (!redir->filename->value)
	{
		print_mem_alloc_error();
		vars->state.status = MISUSE;
		return ;
	}
	concat_str = delete_first_node(concat_str);
	cmd_part->arguments = add_last_arg_list(concat_str, cmd_part->arguments,
			vars, TOKEN_STRING);
}

void	update_arguments(t_command_part  *cmd_part, t_string *concat_str,
		t_vars *vars)
{
	t_arguments	*current_args;

	current_args = cmd_part->arguments;
	while (current_args)
	{
		if (current_args->string->next
			|| current_args->string->type == TOKEN_WILDCARD)
		{
			free_string(current_args->string);
			current_args->string = NULL;
			cmd_part->arguments = add_middle_args(concat_str,
					cmd_part->arguments, vars, TOKEN_STRING);
		}
		current_args = current_args->next;
	}
}