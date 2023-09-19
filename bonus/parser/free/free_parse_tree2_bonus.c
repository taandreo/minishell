/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parse_tree2_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:32:40 by ebezerra          #+#    #+#             */
/*   Updated: 2023/09/15 15:32:41 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	free_argument(t_argument *arg)
{
	if (arg)
	{
		free(arg->value);
		free(arg);
	}
}

void	free_arguments(t_arguments *args)
{
	if (args)
	{
		free_argument(args->argument);
		args->argument = NULL;
		free_arguments(args->next);
		args->next = NULL;
		free(args);
	}
}

void	free_redirection(t_redirection *redir)
{
	if (redir)
	{
		free(redir->filename);
		free(redir);
	}
}

void	free_redirections(t_redirections *redirs)
{
	if (redirs)
	{
		free_redirection(redirs->redirection);
		redirs->redirection = NULL;
		free_redirections(redirs->next);
		redirs->next = NULL;
		free(redirs);
	}
}

void	free_builtin_command(t_builtin_command *cmd)
{
	if (cmd)
	{
		free_arguments(cmd->arguments);
		free(cmd);
	}
}
