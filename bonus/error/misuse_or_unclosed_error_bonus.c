/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misuse_or_unclosed_error_bonus.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 23:32:45 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/15 23:32:48 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	misuse_or_unclosed_quotes_error(t_token_list **tokens)
{
	if (*tokens && (*tokens)->tail)
	{
		if ((*tokens)->tail->token.type == TOKEN_ERROR)
			return (unclosed_quotes_error(tokens));
	}
	if (*tokens)
		free_token_list(tokens);
	return (MISUSE);
}

int	unclosed_quotes_error(t_token_list **tokens)
{
	write(STDERR_FILENO, (*tokens)->tail->token.value,
		ft_strlen((*tokens)->tail->token.value));
	write(STDERR_FILENO, "\n", ft_strlen("\n"));
	free_token_list(tokens);
	return (GENERAL_ERROR);
}

int	unclosed_paren_error(t_token_list **tokens, char **prompt)
{
	write(STDERR_FILENO, "minishell: Unclosed parenthesis\n",
		ft_strlen("minishell: Unclosed parenthesis\n"));
	free_token_list(tokens);
	if (prompt && *prompt)
	{
		free(*prompt);
		*prompt = NULL;
	}
	return (GENERAL_ERROR);
}
