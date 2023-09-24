/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 16:28:51 by tairribe          #+#    #+#             */
/*   Updated: 2023/09/15 15:31:28 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
//#include "ft_printf.h"

void	print_tokens(t_token_list *tokens);
void	print_command(t_command *cmd, size_t indent);
void	print_parse_tree(t_command *parse_tree, size_t indent);

int	main(void)
{
	char			*prompt;
	t_token_list	*tokens;
	t_token_flags	flags;
	t_command		*parse_tree;
	t_parser_state	state;



	while (true)
	{
//		prompt = ft_strdup("$VAR2 echo bla$?\"$VAR2\"$VAR2>$?\"$?\" foo*baz\"baz*\" && \"bar \"<<$? | exit \"$?\" abc");
		prompt = readline("~> ");
		flags = init_token_flags(ft_strlen(prompt));
		tokens = tokenizer(prompt, &flags);
		// Print Tokens
		if (tokens)
		{
			printf("Tokens:\n");
			print_tokens(tokens);
			printf("\n");
			tokens->current = tokens->head;

			parse_tree = parse(tokens, &state);
			// Print parse_tree
			if (parse_tree)
			{
				printf("Parse Tree:\n");
				print_parse_tree(parse_tree, 2);
				free_command(parse_tree);
			}
			free_token_list(&tokens);
		}
		free(prompt);
//		if (flags.status != SUCCESS)
//			exit (flags.status);
		exit (flags.status);
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
				"TOKEN_INVALID",
				"TOKEN_LEFT_PARENTHESIS",
				"TOKEN_RIGHT_PARENTHESIS",
				"TOKEN_REDIRECTIONS",
				"TOKEN_REDIRECTION_INPUT",
				"TOKEN_REDIRECTION_OUTPUT",
				"TOKEN_REDIRECTION_APPEND",
				"TOKEN_REDIRECTION_HEREDOC",
				"TOKEN_ERROR",
				"TOKEN_COMMAND_NAME",
				"TOKEN_FILENAME",
				"TOKEN_WILDCARD",
				"TOKEN_STRING",
				"TOKEN_EXIT_CODE",
				"TOKEN_SPACE",
				"TOKEN_GROUP",
				"TOKEN_NONE",
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

void print_indent(size_t indent)
{
	for (size_t i = 0; i < indent; ++i)
	{
		putchar(' ');
	}
}

void print_redirections(t_redirections *redirections, size_t indent)
{
	while (redirections)
	{
		print_indent(indent);
		printf("%s(\n", token_type_to_string(redirections->redirection->type));
		print_indent(indent + 2);
		printf("%s(%s),\n", token_type_to_string(redirections->redirection->filename->type), redirections->redirection->filename->value);
		print_indent(indent);
		printf("),\n");

		redirections = redirections->next;
	}
}

void print_arguments(t_arguments *arguments, size_t indent)
{
	while (arguments)
	{
		print_indent(indent);
		printf("%s(%s),\n", token_type_to_string(arguments->string->type), arguments->string->value);
		arguments = arguments->next;
	}
}

void print_grouping(t_grouping *grouping, size_t indent)
{
	print_indent(indent);
	printf("GROUP(\n");

	if (grouping->enclosed_cmd)
	{
		print_command(grouping->enclosed_cmd, indent + 2);
	}

	print_indent(indent);
	printf("),\n");
}

void print_pipeline(t_pipeline *pipeline, size_t indent)
{
	while (pipeline)
	{
		print_indent(indent);
		printf("%s(\n", token_type_to_string(pipeline->type));

		switch (pipeline->cmd_part->type)
		{
			case TOKEN_COMMAND_NAME:
				print_indent(indent + 2);
				printf("%s(%s),\n", token_type_to_string(pipeline->cmd_part->u_cmd.cmd_name->type), pipeline->cmd_part->u_cmd.cmd_name->value);
				break;

			case TOKEN_ECHO:  // And other built-in commands...
			case TOKEN_CD:
			case TOKEN_PWD:
			case TOKEN_EXPORT:
			case TOKEN_UNSET:
			case TOKEN_ENV:
			case TOKEN_EXIT:
				print_indent(indent + 2);
				printf("%s(%s),\n", token_type_to_string(pipeline->cmd_part->u_cmd.builtin_cmd->type), pipeline->cmd_part->u_cmd.builtin_cmd->value);
				break;

			default:
				if (pipeline->cmd_part->u_cmd.grouping)
				{
					print_grouping(pipeline->cmd_part->u_cmd.grouping, indent + 2);
				}
				break;
		}

		if (pipeline->cmd_part->arguments)
		{
			print_arguments(pipeline->cmd_part->arguments, indent + 2);
		}

		if (pipeline->cmd_part->redirections)
		{
			print_redirections(pipeline->cmd_part->redirections, indent + 2);
		}

		print_indent(indent);
		printf("),\n");

		pipeline = pipeline->next;
	}
}

void print_conjunctions(t_conjunctions *conjunctions, size_t indent)
{
	while (conjunctions)
	{
		print_indent(indent);
		printf("%s(\n", token_type_to_string(conjunctions->type));

		if (conjunctions->pipeline)
		{
			print_pipeline(conjunctions->pipeline, indent + 2);
		}

		print_indent(indent);
		printf("),\n");

		conjunctions = conjunctions->next;
	}
}

void print_command(t_command *cmd, size_t indent)
{
//	print_indent(indent);
//	printf("COMMAND(\n");

	if (cmd->pipeline)
	{
		print_pipeline(cmd->pipeline, indent + 2);
	}

	if (cmd->conjunctions)
	{
		print_conjunctions(cmd->conjunctions, indent + 2);
	}

//	print_indent(indent);
//	printf("),\n");
}

void print_parse_tree(t_command *parse_tree, size_t indent)
{
	print_indent(indent);
	printf("PARSE_TREE(\n");
	print_command(parse_tree, indent + 2);
	print_indent(indent);
	printf(")\n");
}
