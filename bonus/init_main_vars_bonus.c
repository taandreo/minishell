/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main_vars_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 21:03:36 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/15 21:03:38 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

char	*get_pwd(t_vars *vars)
{
	char	*cwd;
	char	*prompt;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: getcwd: ");
		free_minishell(vars);
		exit(EXIT_FAILURE);
	}
	prompt = ft_strdup("( minishell ):");
	prompt = join_and_cleanup(&prompt, &cwd);
	prompt = join_1st_and_cleanup(&prompt, "$ ");
	return (prompt);
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
	vars->got_cmd_path = false;
	vars->nice_prompt = get_pwd(vars);
	vars->close_heredoc = false;
	vars->is_forked = false;
	vars->state.kill_child = 0;
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
			write(STDERR_FILENO, "minishell: dup2",
				ft_strlen("minishell: dup2"));
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

void	init_fixed_vars(t_vars *vars)
{
	vars->state.is_set = false;
	vars->changed_stdin = false;
	vars->changed_stdout = false;
}
