/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_functions.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:32:52 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/28 21:41:48 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	current_token(const t_token_list *tokens)
{
	t_token	empty_token;

	if (tokens->current)
		return (tokens->current->token);
	else
	{
		empty_token.type = TOKEN_END;
		empty_token.value = NULL;
		return (empty_token);
	}
}

t_token_type	current_token_type(t_token_list *tokens)
{
	return (current_token(tokens).type);
}

void	advance_token(t_token_list *tokens)
{
	if (tokens->current && tokens->current->next)
		tokens->current = tokens->current->next;
}

t_token	peek_token(t_token_list *tokens)
{
	t_token	empty_token;

	if (tokens->current && tokens->current->next)
		return (tokens->current->next->token);
	empty_token.type = TOKEN_END;
	empty_token.value = NULL;
	return (empty_token);
}

void	*null_and_free_grouping(t_grouping *grouping)
{
	free_grouping(grouping);
	return (NULL);
}
