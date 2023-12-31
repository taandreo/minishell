/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_cmd_name_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 19:38:11 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/28 21:43:22 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	add_cmd_name_quotes(char **string, t_token_list **tokens)
{
	if (add_token(tokens, TOKEN_COMMAND_NAME_QUOTES,
			*string) != SUCCESS)
		return (free_nullify_and_return_null(string) != NULL);
	return (true);
}

t_bool	add_cmd_name_unquotes(char **string, t_token_list **tokens)
{
	if (add_token(tokens, TOKEN_COMMAND_NAME_UNQUOTES,
			*string) != SUCCESS)
		return (free_nullify_and_return_null(string) != NULL);
	return (true);
}

t_bool	add_both_cmd_name(t_token_flags *flags, t_token_list **tokens,
		char **string)
{
	if (flags->inside_quotes && flags->quote_type == '\"')
		return (add_cmd_name_quotes(string, tokens));
	else
		return (add_cmd_name_unquotes(string, tokens));
}

t_bool	check_if_is_cmd_true(char *string, t_token_list **tokens)
{
	if ((string && ft_strlen(string) != 0) || !string
		|| ((*tokens)->tail
			&& (*tokens)->tail->token.type == TOKEN_COMMAND_NAME_QUOTES))
		return (true);
	return (false);
}
