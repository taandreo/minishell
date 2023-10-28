/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_string_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:45:15 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/28 21:42:18 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*concat_wildcards(t_string *string, t_vars *vars);

char	*builtin_type_to_value(t_token_type type)
{
	if (type == TOKEN_ECHO)
		return ("echo");
	if (type == TOKEN_CD)
		return ("cd");
	if (type == TOKEN_PWD)
		return ("pwd");
	if (type == TOKEN_EXPORT)
		return ("export");
	if (type == TOKEN_UNSET)
		return ("unset");
	if (type == TOKEN_ENV)
		return ("env");
	if (type == TOKEN_EXIT)
		return ("exit");
	return (NULL);
}

t_string	*delete_first_node(t_string *str)
{
	t_string	*next;

	next = str->next;
	free(str->value);
	free(str);
	return (next);
}

void	*general_error_ambiguous_redirect(t_vars *vars, t_string *string)
{
	char	*variable;

	variable = concat_wildcards(string, vars);
	write(STDERR_FILENO, "minishell: ",
		ft_strlen("minishell: "));
	write(STDERR_FILENO, variable,
		ft_strlen(variable));
	write(STDERR_FILENO, ": ambiguous redirect\n",
		ft_strlen(": ambiguous redirect\n"));
	vars->state.status = GENERAL_ERROR;
	free(variable);
	return (NULL);
}

char	*concat_wildcards(t_string *string, t_vars *vars)
{
	t_string	*current_str;
	char		*concat_str;

	current_str = string;
	concat_str = ft_strdup("");
	if (!concat_str)
	{
		vars->state.status = MISUSE;
		return (return_mem_alloc_error());
	}
	while (current_str)
	{
		concat_str = join_1st_and_cleanup(&concat_str, current_str->value);
		if (!concat_str)
		{
			vars->state.status = MISUSE;
			return (return_mem_alloc_error());
		}
		current_str = current_str->next;
	}
	return (concat_str);
}
