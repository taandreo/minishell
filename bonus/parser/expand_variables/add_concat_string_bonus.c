#include "minishell_bonus.h"

t_arguments	*add_concat_list(t_string *str_list, t_arguments *args,
				t_token_type type);
t_arguments	*add_last_arg_list(t_string *str_list, t_arguments *args,
				t_token_type type);
t_arguments	*add_middle_args(t_string *str_list, t_arguments *args,
				t_vars *vars, t_token_type type);

void	add_concat_string(t_command_part *cmd_part, t_string *concat_str,
		t_vars *vars, t_token_type type)
{
	t_arguments	*current_args;
	t_redirection	*redir;

	if (type == TOKEN_COMMAND_NAME)
	{
		if (is_builtin(concat_str->value))
		{
			free_string(cmd_part->u_cmd.cmd_name);
			cmd_part->u_cmd.cmd_name = NULL;
			cmd_part->type = get_builtin_token(concat_str->value);
			cmd_part->u_cmd.builtin_cmd = malloc(sizeof(t_builtin_cmd));
			cmd_part->u_cmd.builtin_cmd->type = cmd_part->type;
			cmd_part->u_cmd.builtin_cmd->value = ft_strdup(
					builtin_type_to_value(cmd_part->type));
			if (!cmd_part->u_cmd.builtin_cmd->value)
			{
				print_mem_alloc_error();
				vars->state.status = MISUSE;
			}
			concat_str = delete_first_node(concat_str);
			cmd_part->arguments = add_concat_list(concat_str, cmd_part->arguments, TOKEN_STRING);
		}
		else
		{
			free_string(cmd_part->u_cmd.cmd_name->next);
			cmd_part->u_cmd.cmd_name->next = NULL;
			cmd_part->u_cmd.cmd_name->value = ft_strdup(concat_str->value);
			concat_str = delete_first_node(concat_str);
			cmd_part->arguments = add_concat_list(concat_str, cmd_part->arguments, TOKEN_STRING);
		}
	}
	else if (type == TOKEN_STRING)
	{
		current_args = cmd_part->arguments;
		while (current_args)
		{
			if (current_args->string->next)
			{
				free_string(current_args->string);
				current_args->string = NULL;
				cmd_part->arguments = add_middle_args(concat_str, cmd_part->arguments, vars, TOKEN_STRING);
			}
			current_args = current_args->next;
		}
	}
	else if (type == TOKEN_FILENAME)
	{
		redir = cmd_part->redirections->redirection;
		free_string(redir->filename->next);
		free(redir->filename->value);
		redir->filename->next = NULL;
		redir->filename->value = ft_strdup(concat_str->value);
		concat_str = delete_first_node(concat_str);
		cmd_part->arguments = add_last_arg_list(concat_str, cmd_part->arguments, TOKEN_STRING);
	}
}

t_arguments *init_args(t_string *str, t_token_type type)
{
	t_arguments *args;

	args = malloc(sizeof(t_arguments));
	if (!args)
		return (return_mem_alloc_error());
	args->type = type;
	args->string = malloc(sizeof(t_string));
	if (!args->string)
		return (return_mem_alloc_error());
	args->string->type = type;
	args->string->value = ft_strdup(str->value);
	args->string->next = NULL;
	args->next = NULL;
	return (args);
}

t_arguments	*add_concat_list(t_string *str_list, t_arguments *args,
		t_token_type type)
{
	t_string	*current_str;
	t_arguments *first_args;
	t_arguments *last_args;
	t_arguments *c_args;

	current_str = str_list;
	first_args = NULL;
	c_args = NULL;
	last_args = NULL;
	while (current_str)
	{
		c_args = init_args(current_str, type);
		if (!c_args)
		{
			if (args)
				free_arguments(args);
			free_string(str_list);
			return (NULL);
		}
		if (!first_args)
			first_args = c_args;
		if (last_args)
			last_args->next = c_args;
		last_args = c_args;
		current_str = delete_first_node(current_str);
	}
	if (last_args)
		last_args->next = args;
	if (first_args)
		return (first_args);
	return (args);
}

t_arguments	*add_last_arg_list(t_string *str_list, t_arguments *args,
		t_token_type type)
{
	t_string *current_str;
	t_arguments *new_arg;
	t_arguments *last_args;

	current_str = str_list;
	new_arg = NULL;
	last_args = args;
	if (last_args)
	{
		while (last_args->next)
			last_args = last_args->next;
	}
	while (current_str)
	{
		new_arg = init_args(current_str, type);
		if (!new_arg)
		{
			if (args)
				free_arguments(args);
			free_string(str_list);
			return (NULL);
		}
		if (!last_args)
		{
			args = new_arg;
			last_args = new_arg;
		}
		else
		{
			last_args->next = new_arg;
			last_args = new_arg;
		}
		current_str = delete_first_node(current_str);
	}
	return (args);
}

t_arguments	*add_middle_args(t_string *str_list, t_arguments *args,
				t_vars *vars, t_token_type type)
{
	t_arguments *current_args;
	t_arguments *prev_args;
	t_arguments *post_args;
	t_arguments *new_args_list;
	t_arguments *to_be_added;

	post_args = NULL;
	new_args_list = NULL;
	while (str_list)
	{
		to_be_added = init_args(str_list, type);
		if (!to_be_added)
		{
			vars->state.status = MISUSE;
			return (NULL);
		}
		if (!new_args_list)
		{
			new_args_list = to_be_added;
			prev_args = new_args_list;
		}
		new_args_list->next = to_be_added;
		new_args_list = new_args_list->next;
		str_list = delete_first_node(str_list);
	}
	current_args = args;
	while (current_args->next->string)
		current_args = current_args->next;
	post_args = current_args->next->next;
	new_args_list->next = post_args;
	current_args->next = prev_args;
	return (args);
}
