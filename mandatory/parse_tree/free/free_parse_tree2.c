/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parse_tree2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 15:32:40 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/28 21:42:06 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_string(t_string *str)
{
	if (!str)
		return ;
	free(str->value);
	free_string(str->next);
	free(str);
}

void	free_arguments(t_arguments *args)
{
	if (!args)
		return ;
	free_string(args->string);
	free_arguments(args->next);
	free(args);
}

void	free_redirection(t_redirection *redir)
{
	if (!redir)
		return ;
	free_string(redir->filename);
	free(redir);
}

void	free_redirections(t_redirections *redirs)
{
	if (!redirs)
		return ;
	free_redirection(redirs->redirection);
	free_redirections(redirs->next);
	free(redirs);
}

void	free_builtin_command(t_builtin_cmd *cmd)
{
	if (!cmd)
		return ;
	free(cmd->value);
	free(cmd);
}
