/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:31:58 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/28 21:43:50 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	is_builtin(char *token)
{
	if (ft_strcmp(token, "echo") == 0
		|| ft_strcmp(token, "cd") == 0
		|| ft_strcmp(token, "pwd") == 0
		|| ft_strcmp(token, "export") == 0
		|| ft_strcmp(token, "unset") == 0
		|| ft_strcmp(token, "env") == 0
		|| ft_strcmp(token, "exit") == 0)
		return (true);
	return (false);
}

t_bool	decrease_len(t_token_flags *flags)
{
	if (flags->var_len > -1)
		flags->var_len--;
	return (true);
}

t_bool	is_token_cmd_name(t_token_type token)
{
	if (token == TOKEN_COMMAND_NAME_UNQUOTES
		|| token == TOKEN_COMMAND_NAME_QUOTES)
		return (true);
	return (false);
}

char	*get_var_value(char *input, size_t *pos, size_t dollar,
		t_token_flags *flags)
{
	char	*var;

	if (*pos == dollar && input[*pos + 1]
		&& input[*pos + 1] == flags->quote_type)
	{
		var = ft_strndup(input + dollar, *pos + 1 - dollar);
		(*pos)++;
	}
	else
		var = ft_strndup(input + dollar, *pos - dollar);
	return (var);
}
