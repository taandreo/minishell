/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_cmd_name_token_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 19:38:11 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/15 19:38:14 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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
