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
	char *prompt;

	t_grammar	grammar;

	init_grammar(&grammar);
	while (true)
	{
		prompt = readline("~> ");
		tokenizer_bonus(prompt, &grammar);
		free(prompt);
	}
	return 0;
}

