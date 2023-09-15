/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_errors_helper_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:33:04 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:33:05 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	*return_mem_alloc_error(void)
{
	ft_dprintf(STDERR_FILENO, "Error: Memory allocation failed\n");
	return (NULL);
}

void	*return_syntax_error(const char *value)
{
	ft_dprintf(STDERR_FILENO,
		"minishell: syntax error near unexpected token `%s'\n", value);
	return (NULL);
}

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
	ft_dprintf(STDERR_FILENO, "%s\n", (*tokens)->tail->token.value);
	free_token_list(tokens);
	return (GENERAL_ERROR);
}

int	unclosed_paren_error(t_token_list **tokens, char **prompt)
{
	ft_dprintf(STDERR_FILENO, "minishell: Unclosed parenthesis\n");
	free_token_list(tokens);
	if (prompt && *prompt)
		free(*prompt);
	return (GENERAL_ERROR);
}
