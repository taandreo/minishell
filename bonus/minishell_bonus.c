/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 16:28:51 by tairribe          #+#    #+#             */
/*   Updated: 2023/07/09 22:54:21 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "ft_printf.h"



int	main(void)
{
	char			*prompt;
	t_grammar		grammar;
	t_parse_tree	*parse_tree;

	init_grammar(&grammar);
	parse_tree = ft_calloc(1, sizeof(parse_tree));
	init_parse_tree(parse_tree);
	while (true)
	{
		prompt = readline("~> ");
		tokenizer(prompt, &grammar, parse_tree);
		free(prompt);
	}
	return 0;
}

