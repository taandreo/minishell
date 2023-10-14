/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 16:28:51 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/13 22:13:02 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
//#include "ft_printf.h"

void	print_tokens(t_token_list *tokens);
void	print_command(t_command *cmd, size_t indent);
void	print_parse_tree(t_command *parse_tree, size_t indent);
void	init_vars(t_vars *vars);

t_vars	g_vars;

char *get_pwd(void)
{
	char	*pwd;
	char	*blue;
	char	*green;
	char	*prompt;

	pwd = get_env("PWD");
	if (!pwd)
		return ("~> ");
	blue = "\033[01;34m";
	green = "\033[01;00m(\033[01;31m minishell \033[01;00m) \033[00m:";
	prompt = ft_strjoin(green, blue);
	prompt = join_1st_and_cleanup(&prompt, pwd);
	prompt = join_1st_and_cleanup(&prompt, "\033[00m$ ");
	return (prompt);
}



int	main(void)
{
	char			*prompt;
	t_token_flags	flags;
	t_token_list	*tokens;
	t_command		*parse_tree;
	extern char **environ;

	trigger_parent_sigusr();
	init_env(environ);
	g_vars.state.is_set = false;
	g_vars.changed_stdin = false;
	g_vars.changed_stdout = false;
	while (true)
	{
		start_signals_parent();
		init_vars(&g_vars);
		prompt = readline(g_vars.nice_prompt);
		if (!prompt)
		{
			free_minishell(&g_vars);
			write(STDERR_FILENO, "exit\n", ft_strlen("exit\n"));
			exit(g_vars.state.status);
		}
		add_history(prompt);
		g_vars.prompt = &prompt;
		flags = init_token_flags(ft_strlen(prompt));
		tokens = tokenizer(prompt, &flags);
		g_vars.tokens = &tokens;

//		printf("Tokens:\n");
//		print_tokens(*vars.tokens);
//		printf("\n");
		if (flags.status != SUCCESS)
		{
			free_mini_line(&g_vars);
			g_vars.state.is_set = true;
			g_vars.state.status = flags.status;
			continue ;
		}
		// Print Tokens
		if (g_vars.tokens)
		{
			if (tokens->head == tokens->tail)
			{
				if (tokens->head->token.type == TOKEN_END)
				{
					free_mini_line(&g_vars);
					continue ;
				}
			}
			tokens->current = tokens->head;
			parse_tree = parse(*g_vars.tokens, &g_vars.state);
			g_vars.parse_tree = &parse_tree;
			g_vars.args = NULL;
			// Print parse_tree
			if (*g_vars.parse_tree)
			{
//				printf("Parse Tree:\n");
				execute_command(g_vars.parse_tree, &g_vars);
				if (g_vars.state.status == 100)
				{
					free_mini_line(&g_vars);
					exit (g_vars.state.status);
				}
//				if (*vars.parse_tree)
//					print_parse_tree(*vars.parse_tree, 2);
			}
		}
		free_mini_line(&g_vars);
	}
	exit (g_vars.state.status);
	return (SUCCESS);
}



void	init_vars(t_vars *vars)
{
	vars->tokens = NULL;
	vars->parse_tree = NULL;
	vars->args = NULL;
	vars->prompt = NULL;
	if (vars->changed_stdin)
	{
		restore_stdin(vars->saved_stdin, vars);
		vars->changed_stdin = false;
	}
	if (vars->changed_stdout)
	{
		restore_stdout(vars->saved_stdout, vars);
		vars->changed_stdout = false;
	}
	vars->nice_prompt = get_pwd();
	vars->close_heredoc = false;
	vars->saved_stdin = init_stdin_var(vars);
	vars->saved_stdout = init_stdout_var(vars);
}

int	init_stdin_var(t_vars *vars)
{
	if (!vars->changed_stdin)
	{
		vars->saved_stdin = dup(STDIN_FILENO);
		if (vars->saved_stdin == -1)
		{
			write(STDERR_FILENO, "minishell: dup2", ft_strlen("minishell: dup2"));
			perror("");
			vars->state.error = true;
			vars->state.is_set = true;
			vars->state.status = GENERAL_ERROR;
		}
	}
	return (vars->saved_stdin);
}

int	init_stdout_var(t_vars *vars)
{
	if (!vars->changed_stdout)
	{
		vars->saved_stdout = dup(STDIN_FILENO);
		if (vars->saved_stdin == -1)
		{
			write(STDERR_FILENO, "minishell: dup2",
					ft_strlen("minishell: dup2"));
			perror("");
			vars->state.error = true;
			vars->state.is_set = true;
			vars->state.status = GENERAL_ERROR;
		}
	}
	return (vars->saved_stdout);
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
		t_string *filename = redirections->redirection->filename;
		print_indent(indent + 2);
		while (filename)
		{
			printf("%s(%s), ", token_type_to_string(filename->type), filename->value);
			filename = filename->next;
		}
		printf("\n");
		print_indent(indent);
		printf("),\n");

		redirections = redirections->next;
	}
}

void print_arguments(t_arguments *arguments, size_t indent)
{
	while (arguments)
	{
		t_string *string = arguments->string;
		print_indent(indent);
		while (string)
		{
			printf("%s(%s), ", token_type_to_string(string->type), string->value);
			string = string->next;
		}
		printf("\n");
		print_indent(indent);
		printf("),\n");
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

		t_string *cmd_name = pipeline->cmd_part->u_cmd.cmd_name;
		switch (pipeline->cmd_part->type)
		{
			case TOKEN_COMMAND_NAME:
				print_indent(indent + 2);
				printf("%s(\n", token_type_to_string(pipeline->cmd_part->type));
				print_indent(indent + 4);
				while (cmd_name)
				{
					printf("%s(%s), ", token_type_to_string(cmd_name->type), cmd_name->value);
					cmd_name = cmd_name->next;
				}
				printf("\n");
				print_indent(indent + 2);
				printf("),\n");
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
			if (pipeline->cmd_part->type == TOKEN_REDIRECTIONS)
			{
				print_indent(indent + 2);
				printf("%s(\n", token_type_to_string(pipeline->cmd_part->type));
				print_redirections(pipeline->cmd_part->redirections, indent + 4);
				print_indent(indent + 2);
				printf("),\n");
			}
			else
				print_redirections(pipeline->cmd_part->redirections, indent + 2);
		}

		print_indent(indent);
		printf("),\n");

		pipeline = pipeline->next;
	}
}

void print_conjunctions(t_conjunctions *conjunctions, size_t indent)
{
	t_conjunctions *conj = conjunctions;
	while (conj)
	{
		print_indent(indent);
		printf("%s(\n", token_type_to_string(conj->type));

		if (conj->pipeline)
		{
			print_pipeline(conj->pipeline, indent + 2);
		}

		print_indent(indent);
		printf("),\n");

		conj = conj->next;
	}
}

void print_command(t_command *cmd, size_t indent)
{
	if (cmd->pipeline)
	{
		print_pipeline(cmd->pipeline, indent + 2);
	}

	if (cmd->conjunctions)
	{
		print_conjunctions(cmd->conjunctions, indent + 2);
	}
}

void print_parse_tree(t_command *parse_tree, size_t indent)
{
	print_indent(indent);
	printf("PARSE_TREE(\n");
	print_command(parse_tree, indent + 2);
	print_indent(indent);
	printf(")\n");
}
