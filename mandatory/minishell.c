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

void	print_parse_tree(t_command *parse_tree, size_t indent);
void	print_tokens(t_token_list *tokens);
void	print_command(t_command *cmd, size_t indent);
void	init_vars(t_vars *vars);

t_vars	g_vars;

int	launch_minishell(char *prompt)
{
	t_token_flags	flags;
	t_token_list	*tokens;

	flags = init_token_flags(ft_strlen(prompt));
	tokens = tokenizer(prompt, &flags);
	g_vars.tokens = &tokens;
	if (flags.status != SUCCESS)
	{
		free_mini_line(&g_vars);
		g_vars.state.is_set = true;
		g_vars.state.status = flags.status;
		return (g_vars.state.status);
	}
	if (g_vars.tokens)
	{
		if (empty_token_list(tokens))
			return (g_vars.state.status);
		tokens->current = tokens->head;
		init_parse_tree_and_execute_cmd(&g_vars);
	}
	free_mini_line(&g_vars);
	return (g_vars.state.status);
}

void	run_without_loop(int argc, char **argv)
{
	int	exit_status;

	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
	{
		exit_status = launch_minishell(argv[2]);
		exit(exit_status);
	}
}

int	main(int argc, char **argv)
{
	char		*prompt;
	extern char	**environ;

	trigger_parent_sigusr();
	init_env(environ);
	init_fixed_vars(&g_vars);
	run_without_loop(argc, argv);
	while (true)
	{
		start_signals_parent();
		init_vars(&g_vars);
		prompt = readline(g_vars.nice_prompt);
		if (!prompt)
		{
			free_minishell(&g_vars);
			write_proper_msg();
			break ;
		}
		add_history(prompt);
		g_vars.prompt = &prompt;
		launch_minishell(prompt);
	}
	return (g_vars.state.status);
}
