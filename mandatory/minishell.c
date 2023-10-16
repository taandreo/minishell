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

int	launch_minishell(char *prompt, t_vars *vars, t_bool without_loop)
{
	t_token_flags	flags;
	t_token_list	*tokens;

	if (without_loop)
		init_vars(vars);
	flags = init_token_flags(ft_strlen(prompt));
	tokens = tokenizer(prompt, &flags);
	vars->tokens = &tokens;
	if (flags.status != SUCCESS)
	{
		free_mini_line(&g_vars);
		vars->state.is_set = true;
		vars->state.status = flags.status;
		return (vars->state.status);
	}
	if (vars->tokens && *vars->tokens)
	{
		if (empty_token_list(tokens))
			return (vars->state.status);
		tokens->current = tokens->head;
		init_parse_tree_and_execute_cmd(vars);
	}
	free_mini_line(vars);
	return (vars->state.status);
}

void	run_without_loop(int argc, char **argv, t_vars *vars)
{
	int		exit_status;
	t_bool	without_loop;

	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
	{
		without_loop = true;
		exit_status = launch_minishell(argv[2], vars, without_loop);
		exit(exit_status);
	}
}

int	main(int argc, char **argv)
{
	char		*prompt;
	extern char	**environ;

	init_env(environ);
	init_fixed_vars(&g_vars);
	run_without_loop(argc, argv, &g_vars);
	while (true)
	{
		start_signals_parent();
		trigger_parent_sigusr();
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
		g_vars.state.status = launch_minishell(prompt, &g_vars,
				false);
	}
	return (g_vars.state.status);
}
