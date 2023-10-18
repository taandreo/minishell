/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_and_concat_exit_code_bonus.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:54:17 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/15 23:55:13 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_string	*expand_exit_code(t_string	*string,
	t_vars *vars, t_token_type type)
{
	t_string	*str;

	str = string;
	while (str)
	{
		if (str->type == TOKEN_EXIT_CODE)
		{
			str->type = type;
			free(str->value);
			str->value = ft_itoa(vars->state.status);
			if (!str->value)
			{
				free_string(string);
				vars->state.status = MISUSE;
				vars->state.error = true;
				vars->state.is_set = true;
				return (return_mem_alloc_error());
			}
		}
		str = str->next;
	}
	return (string);
}

t_string	*concat_exit_code(t_string *string, t_vars *vars, t_token_type type)
{
	t_string	*current;

	current = string;
	while (current)
	{
		if (current->type == type)
		{
			while (peek_string_type(current) == type)
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

t_string	*clean_next_token(t_string *string)
{
	t_string	*next;

	if (string == NULL || string->next == NULL)
		return (NULL);
	next = string->next;
	string->next = next->next;
	free(next->value);
	free(next);
	return (string->next);
}

t_token_type	peek_string_type(t_string	*string)
{
	if (string && string->next)
		return (string->next->type);
	return (TOKEN_END);
}
