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
	t_token_list	tokens;
	size_t			input_len;
	t_token_flags	flags;

	while (true)
	{
		prompt = readline("~> ");
		flags = init_flags(ft_strlen(prompt));
		tokens = tokenizer(prompt, &flags);
		free(prompt);
	}
	return 0;
}

