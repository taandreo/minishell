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

t_command	*parse(t_token_list *tokens)
{
	t_command	*parse_tree;

	parse_tree = parse_command(tokens);
	if (!parse_tree || current_token_type(tokens) != TOKEN_END)
	{
		if (parse_tree)
		{
			free_command(parse_tree);
			return_syntax_error(current_token(tokens).value);
		}
		exit(MISUSE);
	}
	return (parse_tree);
}
