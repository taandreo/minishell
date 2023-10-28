/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_args_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:50:10 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/28 21:42:13 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_arguments	*init_args(t_string *str, t_token_type type)
{
	t_arguments	*args;

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

t_arguments	*add_first_args(t_string *str_list, t_arguments *args,
				t_vars *vars, t_token_type type)
{
	t_arguments	*first_args;
	t_arguments	*last_args;
	t_arguments	*c_args;

	first_args = NULL;
	c_args = NULL;
	last_args = NULL;
	while (str_list)
	{
		c_args = init_args(str_list, type);
		if (!c_args)
			return (null_free_args_misuse(args, first_args, str_list, vars));
		if (!first_args)
			first_args = c_args;
		if (last_args)
			last_args->next = c_args;
		last_args = c_args;
		str_list = delete_first_node(str_list);
	}
	if (last_args)
		last_args->next = args;
	if (first_args)
		return (first_args);
	return (args);
}

t_arguments	*add_last_arg_list(t_string *str_list, t_arguments *args,
				t_vars *vars, t_token_type type)
{
	t_arguments	*new_arg;
	t_arguments	*last_args;

	new_arg = NULL;
	last_args = args;
	if (last_args)
		walk_last_args(last_args);
	while (str_list)
	{
		new_arg = init_args(str_list, type);
		if (!new_arg)
			return (null_free_args_misuse(args, NULL, str_list, vars));
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
		str_list = delete_first_node(str_list);
	}
	return (args);
}

t_bool	add_middle_into_args(t_arguments *current_args,
			t_arguments *new_args_list, t_arguments *prev_args)
{
	t_arguments	*post_args;

	post_args = NULL;
	if (current_args->next)
	{
		while (current_args->next->string)
			current_args = current_args->next;
		post_args = current_args->next->next;
	}
	new_args_list->next = post_args;
	if (current_args->string)
		current_args->next = prev_args;
	else
		return (false);
	return (true);
}

t_arguments	*add_middle_args(t_string *str_list, t_arguments *args,
		t_vars *vars, t_token_type type)
{
	t_arguments	*current_args;
	t_arguments	*prev_args;
	t_arguments	*new_args_list;
	t_arguments	*to_be_added;

	new_args_list = NULL;
	while (str_list)
	{
		to_be_added = init_args(str_list, type);
		if (!to_be_added)
			return (null_free_args_misuse(NULL, prev_args, str_list, vars));
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
	if (!add_middle_into_args(current_args, new_args_list, prev_args))
		args = prev_args;
	return (args);
}
