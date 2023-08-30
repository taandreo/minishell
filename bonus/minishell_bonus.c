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

void	print_tokens(t_token_list *tokens);

int	main(void)
{
	char			*prompt;
	t_token_list	*tokens;
	t_token_flags	flags;
//	t_command		*parse_tree;

	while (true)
	{
//		prompt = ft_strdup("echo a\"e$? $HOME/\"teste");
		prompt = readline("~> ");
		flags = init_flags(ft_strlen(prompt));
		tokens = tokenizer(prompt, &flags);
//		tokens->current = tokens->head;
//		parse_tree = parse(&tokens);
		// Test tokens
		if (tokens)
			print_tokens(tokens);
		free_token_list(&tokens);
		free(prompt);
		return 0;
	}
	return (SUCCESS);
}

const char *token_names[] =
		{
				"TOKEN_ECHO",
				"TOKEN_CD",
				"TOKEN_PWD",
				"TOKEN_EXPORT",
				"TOKEN_UNSET",
				"TOKEN_ENV",
				"TOKEN_EXIT",
				"TOKEN_AND",
				"TOKEN_OR",
				"TOKEN_PIPE",
				"TOKEN_REDIRECTIONS",
				"TOKEN_REDIRECTION_INPUT",
				"TOKEN_REDIRECTION_OUTPUT",
				"TOKEN_REDIRECTION_APPEND",
				"TOKEN_REDIRECTION_HEREDOC",
				"TOKEN_COMMAND_NAME",
				"TOKEN_FILENAME",
				"TOKEN_STRING",
				"TOKEN_EXIT_CODE",
				"TOKEN_SPACE",
				"TOKEN_SPECIAL_ARG",
				"TOKEN_INVALID",
				"TOKEN_ERROR",
				"TOKEN_END"
		};

// Function to convert token type to string
const char *token_type_to_string(t_token_type type)
{
	// Check if type is out of bounds
	if (type < 0 || type >= sizeof(token_names) / sizeof(token_names[0]))
	{
		return "Invalid token type";
	}
	return token_names[type];
}


void	print_tokens(t_token_list *tokens)
{
	t_token_node *current = tokens->head;

	while (current)
	{
		printf("%s(%s)\n", token_type_to_string(current->token.type), current->token.value);
		current = current->next;
	}
}
