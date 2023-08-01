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

int	main(void)
{
	char *prompt;

	t_grammar	*grammar;
	while (true)
	{
		prompt = readline("~> ");
		tokenizer(grammar, prompt);
		free(prompt);
	}
	return 0;
}