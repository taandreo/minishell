/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 16:28:51 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/10 19:26:46 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
//#include "ft_printf.h"

void	print_tokens(t_token_list *tokens);
void	print_command(t_command *cmd, size_t indent);
void	print_parse_tree(t_command *parse_tree, size_t indent);
void	init_vars(t_vars *vars);

t_list *g_env;
int g_exit_status = 0;

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

void parent_handler(int signum)
{
	(void)signum;
}

void child_handler(int signum)
{
	if (signum == SIGUSR1)
		g_exit_status = 100;
}

int	main(void)
{
	char			*prompt;
	t_vars			vars;
	t_token_flags	flags;
	t_token_list	*tokens;
	t_command		*parse_tree;
	struct sigaction sa_parent;
	sa_parent.sa_handler = parent_handler;
	sa_parent.sa_flags = 0;
	sigemptyset(&sa_parent.sa_mask);

	if (sigaction(SIGUSR1, &sa_parent, NULL) == -1) {
		perror("Error setting up parent signal handler");
		exit(EXIT_FAILURE);
	}
	extern char **environ;
	init_env(environ);
	vars.state.is_set = false;
	vars.changed_stdin = false;
	vars.changed_stdout = false;
	while (true)
	{
		init_vars(&vars);
		prompt = readline(vars.nice_prompt);
		add_history(prompt);
		vars.prompt = &prompt;
		flags = init_token_flags(ft_strlen(prompt));
		tokens = tokenizer(prompt, &flags);
		vars.tokens = &tokens;

//		printf("Tokens:\n");
//		print_tokens(*vars.tokens);
//		printf("\n");
		if (flags.status != SUCCESS)
		{
			free_minishell(&vars);
			vars.state.is_set = true;
			vars.state.status = flags.status;
			continue ;
		}
		// Print Tokens
		if (vars.tokens)
		{
			if (tokens->head == tokens->tail)
			{
				if (tokens->head->token.type == TOKEN_END)
				{
					free_minishell(&vars);
					continue ;
				}
			}
			tokens->current = tokens->head;
			parse_tree = parse(*vars.tokens, &vars.state);
			vars.parse_tree = &parse_tree;
			vars.args = NULL;
			// Print parse_tree
			if (*vars.parse_tree)
			{
//				printf("Parse Tree:\n");
				execute_command(vars.parse_tree, &vars);
				if (g_exit_status == 100)
				{
					free_minishell(&vars);
					exit (vars.state.status);
				}
//				if (*vars.parse_tree)
//					print_parse_tree(*vars.parse_tree, 2);
			}
		}
		free_minishell(&vars);
	}
	exit (vars.state.status);
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
	vars->is_forked = false;
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
