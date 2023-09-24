/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:32:11 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:32:12 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	advance_space(char *input, size_t *pos, t_token_list **tokens,
			size_t input_len);
void	add_token_end(t_token_list **tokens, t_token_flags *flags,
			char **prompt);
t_bool	create_flags_prompt(t_token_flags *flags, t_token_list **tokens,
			char *input);

t_token_list	*tokenizer(char *input, t_token_flags *flags)
{
	t_token_list	*tokens;
	size_t			pos;
	char			c;

	pos = 0;
	tokens = create_token_list();
	if (!create_flags_prompt(flags, &tokens, input))
		return (return_mem_alloc_error());
	while (pos < ft_strlen(flags->input))
	{
		c = flags->input[pos];
		if (ft_is_space(c))
		{
			advance_space(flags->input, &pos, &tokens, ft_strlen(flags->input));
			continue ;
		}
		flags->status = tokenize_by_category(&flags->input, &pos, &tokens,
				flags);
		if (flags->status != SUCCESS)
			break ;
	}
	if (tokens && flags->paren_count)
		flags->status = unclosed_paren_error(&tokens, &flags->input);
	add_token_end(&tokens, flags, &flags->input);
	return (tokens);
}

t_bool	create_flags_prompt(t_token_flags *flags, t_token_list **tokens,
			char *input)
{
	flags->input = ft_strdup(input);
	if (!flags->input)
	{
		free_token_list(tokens);
		return (false);
	}
	return (true);
}

void	advance_space(char *input, size_t *pos, t_token_list **tokens,
			size_t input_len)
{
	if (!ft_strchr("><|&() ", peek_next(input, *pos, input_len))
		&& !((*tokens)->tail->token.type >= TOKEN_AND
			&& (*tokens)->tail->token.type <= TOKEN_REDIRECTION_HEREDOC))
	{
		if (add_token(tokens, TOKEN_SPACE, " ") != SUCCESS)
			ft_dprintf(STDERR_FILENO, "Error: Memory allocation failed\n");
	}
	(*pos)++;
}

void	add_token_end(t_token_list **tokens, t_token_flags *flags,
			char **prompt)
{
	if (flags->status == SUCCESS)
		if (add_token(tokens, TOKEN_END, "") != SUCCESS)
			ft_dprintf(STDERR_FILENO, "Error: Memory allocation failed\n");
	if (prompt && *prompt)
	{
		free(*prompt);
		*prompt = NULL;
	}
}
