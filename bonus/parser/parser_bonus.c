/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:32:49 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:32:50 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

t_command	*parse(t_token_list *tokens, t_parser_state *state)
{
	t_command	*parse_tree;

	state->paren_count = 0;
	state->status = SUCCESS;
	state->has_paren = false;
	state->error = false;
	parse_tree = parse_command(tokens, state);
	if (!parse_tree || current_token_type(tokens) != TOKEN_END)
	{
		if (state->status != MISUSE)
		{
			if (current_token_type(tokens) == TOKEN_SPACE)
				advance_token(tokens);
			return_syntax_error(current_token(tokens).value);
		}
		if (parse_tree && current_token_type(tokens) != TOKEN_END)
		{
			free_command(parse_tree);
			parse_tree = NULL;
		}
		state->status = MISUSE;
	}
	return (parse_tree);
}
