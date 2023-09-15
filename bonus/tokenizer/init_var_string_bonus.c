/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_var_string_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:31:02 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:31:19 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	*initialize_var_string(char *input, size_t pos,
			t_token_flags *flags, const char *start)
{
	char	*tmp;

	flags->init_var = true;
	if (flags->inside_quotes)
	{
		pos++;
		while (input[pos] && input[pos] != flags->quote_type)
			pos++;
		while (input[pos] && is_string_start(input[pos], flags))
			pos++;
	}
	else
	{
		while (input[pos] && is_string_start(input[pos], flags))
			pos++;
	}
	flags->init_var = false;
	tmp = ft_strndup(start, input + pos - start);
	if (!tmp)
		return (free_and_return_null(flags->string));
	flags->var = ft_strjoin(flags->string, tmp);
	if (!flags->var)
		return (free_and_return_null(flags->string));
	free(tmp);
	return (flags->var);
}
