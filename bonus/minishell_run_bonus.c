/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_run_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 21:03:42 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/15 21:03:44 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	init_parse_tree_and_execute_cmd(t_vars *vars)
{
	t_command		*parse_tree;

	parse_tree = parse(*vars->tokens, &vars->state);
	vars->parse_tree = parse_tree;
	vars->args = NULL;
	if (vars->parse_tree)
	{
		if (isatty(STDIN_FILENO))
		{
			execute_command(&vars->parse_tree, vars);
			if (vars->state.kill_child == 1)
			{
				free_minishell(&g_vars);
				exit(g_vars.state.status);
			}
		}
		else
		{
			execute_command(&vars->parse_tree, vars);
			free_minishell(vars);
			exit(vars->state.status);
		}
	}
}

t_bool	empty_token_list(t_token_list *tokens)
{
	if (tokens->head == tokens->tail)
	{
		if (tokens->head->token.type == TOKEN_END)
		{
			free_mini_line(&g_vars);
			return (true);
		}
	}
	return (false);
}

void	write_proper_msg(void)
{
	if (isatty(STDIN_FILENO))
		write(STDERR_FILENO, "exit\n", ft_strlen("exit\n"));
	else
		write(STDERR_FILENO, "\n", 1);
}
